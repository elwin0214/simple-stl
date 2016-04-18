/*

http://blog.csdn.net/gabriel1026/article/details/6311339
*/

#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <cmath>
#include "allocator.hpp"
#include "data_fill.hpp"
#include <iostream>
#include <stdio.h>

#define LEFT_SUB_HEIGHT(node) ((node)->left == NULL ? 0 : (node->left)->height)
#define RIGHT_SUB_HEIGHT(node) ((node)->right == NULL ? 0 : (node->right)->height)

#define IS_ROOT_NODE(node) (head_->parent == (node))
//#define IS_ROOT_NODE(node) (root_ == (node))

#define IS_HEAD_NODE(node) (head_ == (node))

#define EXIT_ROOT() (head_->parent != NULL)

#define IS_FIRST_NODE(node) (head_->left == (node))
#define IS_LAST_NODE(node) (head_->right == (node))


#define IS_LEFT_CHILD(node)  ((head_ != (node)->parent) && ((node)->parent->left == (node)))
#define IS_RIGHT_CHILD(node)  ((head_ != (node)->parent) && ((node)->parent->right == (node)))


#define LOG_DEBUG(fmt,...) fprintf(stdout, fmt, ##__VA_ARGS__)

namespace simple{

template <typename Value>
struct treenode 
{

    treenode<Value> *parent;
    treenode<Value> *left;
    treenode<Value> *right;
    Value data;
    int height;
//private 声明 friend avltree  异常
public:
    treenode(const Value& data):data(data),parent(NULL),left(NULL),right(NULL),height(1){
        
    }
};


template <typename Value>
struct treenode_iterator
{
public:

    typedef treenode<Value>* node_ptr_type; 
    typedef treenode<Value> node_value_type; 
    typedef Value value_type; 
    typedef Value* value_ptr_type;
    typedef treenode_iterator self;//?
    typedef treenode_iterator<Value> iterator;


    node_ptr_type node_ptr_;
    node_ptr_type head_;
public:

    void increase()
    {
        if (IS_LAST_NODE(node_ptr_))
        {
            node_ptr_ = head_;
            return;
        }
        node_ptr_type node_ptr = node_ptr_;
        if (node_ptr->right != NULL)// the most left node in right sub-tree
        {
            node_ptr = node_ptr->right;
            while (node_ptr->left != NULL)
                node_ptr = node_ptr->left;
        }
        else 
        {
            if (IS_LEFT_CHILD(node_ptr))//goto parent if it's left chid
            {
                node_ptr = node_ptr->parent;
            }
            else if (IS_RIGHT_CHILD(node_ptr)){//find the node whose most right node in left sub-tree is this node.           {
                while(IS_RIGHT_CHILD(node_ptr))//the node_ptr will not be root node
                {
                    node_ptr = node_ptr->parent;
                }
                node_ptr = node_ptr->parent;
            }
            //root node without right child, will not go here,
            //the right child of the most right node of tree will be assign to the member variable - head_
            else
            {   //#ifdef DEBUG
                LOG_DEBUG("wont go there");
                //#endif
            }
        }
        node_ptr_ = node_ptr;
    }

    // void decrease()
    // {
    //     node_ptr_type node_ptr = node_ptr_;
    //     if (node_ptr->left != NULL)
    //     {
    //         node_ptr = node_ptr->left;
    //         while (node_ptr->right != NULL)
    //             node_ptr = node_ptr->right;
    //     }
    //     else//无左子树
    //     {
    //         if (IS_RIGHT_CHILD(node_ptr))
    //         {
    //             node_ptr = node_ptr->parent;
    //         }
    //         else
    //         {
    //             node_ptr = head_; //已经到了根节点或者最左节点
    //         }
    //         node_ptr = node_ptr->parent;
    //     }
    //     node_ptr_ = node_ptr;
    // }


public:
    treenode_iterator(node_ptr_type head, node_ptr_type node_ptr):head_(head),node_ptr_(node_ptr)
    {

    }

    iterator& operator++()
    {
        increase();
        return *this;
    }

    iterator operator++(int)
    {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const iterator &itr)
    {
        return node_ptr_ == itr.node_ptr_;
    }

    bool operator!=(const iterator &itr)
    {
        return node_ptr_ != itr.node_ptr_;
    }

    value_type& operator*()
    {
        return (*node_ptr_).data;
    }

    value_ptr_type operator->()
    {
        return &(node_ptr_->data);
    }

};


template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
class avltree
{

public:
    typedef treenode<Value>* node_ptr_type; 
    typedef treenode<Value> node_value_type;
    typedef size_t size_type;
    typedef treenode_iterator<Value> iterator;

private:
    typedef allocator<node_value_type,Alloc> _allocator;
    //treenode<Value> *root_;
    treenode<Value> *head_;

    //treenode<Value> *head_;
    //treenode<Value> *tail_;
    Compare compare_;
    KeyOfValue key_of_value_;
    enum SubTreeType
    {
        DIRECT,
        LEFT,
        RIGHT
    };

public:
    avltree():head_(create())
    {
        head_->left = head_;
        head_->right = head_;
    }

    ~avltree()
    {
        //不能类似于list迭代回收内存
        //因为迭代加，会走析构过的内存。
        //iterator itr = begin();
        //iterator end_itr = end();

        // for (; itr != end_itr; )
        // {
        //     iterator tmp = itr;
        //     ++itr;
        //     destory_node(tmp.node_ptr_);
        // }
        // _allocator::deallocate(head_);
    }

    iterator begin()
    {
        return iterator(head_, head_->left);//最左儿子
    }

    iterator end()
    {
        return iterator(head_, head_);
    }

    node_ptr_type getRoot()
    {
        return head_->parent;
    }

    void insert(const Value &value)
    {
        //std::cout<<key_of_value_(value)<<std::endl;
        if (!EXIT_ROOT())
        {
            LOG_DEBUG("insert, root, key=%d\n", key_of_value_(value));
            node_ptr_type node_ptr = create_and_fill(value);
            node_ptr->parent = head_;

            head_->left = node_ptr;
            head_->right = node_ptr;
            head_->parent = node_ptr;
            return;
        }
        node_ptr_type node = getRoot();

        while (node != NULL)
        {

            if (compare_(key_of_value_(value), key_of_value_(node->data)))
            {
                if (node->left != NULL) node = node->left;
                else 
                {
                    LOG_DEBUG("insert, left, key=%d\n", key_of_value_(value));
                    node_ptr_type new_node = add_node_left(node, value);//更新head_->left
                    reset_head_children(new_node);
                    
                    return;
                }
            }
            else
            {
                    LOG_DEBUG("insert, right, key=%d\n", key_of_value_(value));
                    if (node->right != NULL) node = node->right;
                    else
                    {
                        node_ptr_type new_node = add_node_right(node, value);
                        
                        reset_head_children(new_node);
                        return;
                    }
            }
        }
            
    }
private:
    node_ptr_type create()
    {
        return _allocator::allocate(1);
    }

    node_ptr_type create_and_fill(const Value& value) 
    {
        node_ptr_type result = _allocator::allocate(1);
        //construct((&(result->data)), value);//如果直接在成员data上构造，node里面的指针没有初始化，导致有bug
        fill_n(result, 1, value);
        return result;
    }

    void destory_node(node_ptr_type node)
    {
        destroy(&(node->data));
        _allocator::deallocate(node);
    }

    node_ptr_type get_most_left_node(node_ptr_type node)
    {
        while (node->left != NULL)
        {
            node = node->left;
        }
        return node;
    }

    node_ptr_type get_most_right_node(node_ptr_type node)
    {
        while (node->right != NULL && !IS_HEAD_NODE(node))
        {
            node = node->right;
        }
        return node;
    }

    node_ptr_type add_node_left(node_ptr_type node, const Value &t)
    {
        node_ptr_type result = create_and_fill(t);
        node->left = result;
        node->left->parent = node;
        int before_height = node->height;
        int cur_height = recount_height(node);
        if (cur_height == before_height) return result; //无需旋转
        count_and_rotate(node);
        return result;
    }

    node_ptr_type add_node_right(node_ptr_type node, const Value &t)
    {
        node_ptr_type result = create_and_fill(t);
        node->right = result;
        node->right->parent = node;
        int before_height = node->height;
        int cur_height = recount_height(node);
        if (cur_height == before_height) return result; //无需旋转
        count_and_rotate(node);
        return result;
    }

    void count_and_rotate(node_ptr_type node)
    {
        //cout<<"count ratate "<<(node->data)<<endl;
        while (!IS_HEAD_NODE(node)) //node!=NULL
        {
            int lh = LEFT_SUB_HEIGHT(node);//node->left == NULL ? 0 : node->left->height;
            int rh = RIGHT_SUB_HEIGHT(node);//node->right == NULL ? 0 : node->right->height;
            if (abs(lh - rh) >= 2)
            {
                rotate(node, (lh > rh ? LEFT : RIGHT));
                return;//only one node need to be rotate 
            }
            node->height = std::max(lh, rh) + 1;
            node = node->parent;

        }
    }
    int recount_height(node_ptr_type node)
    {
        if (node == NULL) return 0;
        int lh = LEFT_SUB_HEIGHT(node);//->left == NULL ? 0 : node->left->height;
        int rh = RIGHT_SUB_HEIGHT(node);//->right == NULL ? 0 : node->right->height;
        node->height = std::max(lh, rh) + 1;
        return node->height;
    }

    void set_to_ltree(node_ptr_type left, node_ptr_type node)
    {
        if (NULL != node) node->left = left;
        if (NULL != left) left->parent = node;
    }
    void set_to_rtree(node_ptr_type right, node_ptr_type node)
    {
        if (NULL != node) node->right = right;
        if (NULL != right) right->parent = node;
    }

    void set_to_child(node_ptr_type child, node_ptr_type parent, SubTreeType st_ptr)
    {
        
        child->parent = parent;
        if (IS_HEAD_NODE(parent))//新的树根节点  head_ == parent
        {
            head_->parent = child;
            LOG_DEBUG("set_to_child, root, key=%d\n", key_of_value_(child->data));
        }
        else//根据当前是在parent节点下左子树还是右子树 进行替换
        {
            LOG_DEBUG("set_to_child, key=%d, parent=%d\n", key_of_value_(child->data), key_of_value_(parent->data));

            if (LEFT == (st_ptr))
            {
                set_to_ltree(child, parent);
            }
            else if (RIGHT ==(st_ptr))
            {
                set_to_rtree(child, parent);
            }
        }
    }
    /*根据左右子树的高度大小判断旋转方式*/
    void rotate(node_ptr_type node, SubTreeType st)
    {
        LOG_DEBUG("rotate, key=%d\n", key_of_value_(node->data));
        /*当前旋转的节点node 所处的子树是左子树还是右子树*/
        SubTreeType rotate_tree_type = DIRECT;
        //if (NULL != node->parent)//??
        //{
        //the parent node of this sub-tree should choose a new node as child after rotate.
        //but we should know which child it is.
        rotate_tree_type = node->parent->left == node ? (LEFT) : (RIGHT); 
        //}
        if (st == LEFT)//左子树高度大
        {
            node_ptr_type parent = node->parent;
            node_ptr_type left = node->left;
            node_ptr_type left_right = left->right;
            if (LEFT_SUB_HEIGHT(node->left) > RIGHT_SUB_HEIGHT(node->left)) /*LL*/
            {
                //left -> root
                set_to_ltree(left_right, node);
                set_to_rtree(node, left);

                //set_to_ltree(left, parent);
                //if (NULL == parent) root = left;
                set_to_child(left, parent, rotate_tree_type);

                recount_height(left->right);
                recount_height(left);
            }
            else
            {
                //left_right -> root
                set_to_rtree(left_right->left, left);
                set_to_ltree(left_right->right, node);
                set_to_ltree(left, left_right);
                set_to_rtree(node, left_right);

                //set_to_ltree(left_right, parent);
                //if (NULL == parent) root = left_right;
                set_to_child(left_right, parent, rotate_tree_type);

                recount_height(left_right->left);
                recount_height(left_right->right);
                recount_height(left_right);
            }
        }
        else
        {
            node_ptr_type parent = node->parent;
            node_ptr_type right = node->right;
            node_ptr_type right_left = right->left;

            if (LEFT_SUB_HEIGHT(node->right) > RIGHT_SUB_HEIGHT(node->right))
            {
                //right_left -> root
                set_to_ltree(right_left->right, right);
                set_to_rtree(right_left->left, node);
                set_to_ltree(node, right_left);
                set_to_rtree(right, right_left);

                set_to_child(right_left, parent, rotate_tree_type);

                recount_height(right_left->left);
                recount_height(right_left->right);
                recount_height(right_left);
            }
            else
            {
                //right -> root
                set_to_rtree(right_left, node);
                set_to_ltree(node, right);

                set_to_child(right, parent, rotate_tree_type);
                recount_height(right->left);
                recount_height(right);
                //
            }
        }
    }
    //重新设置 树的最左节点和最右节点
    void reset_head_children(node_ptr_type new_node)
    {
        head_->left = get_most_left_node(getRoot());
        head_->right = get_most_right_node(getRoot());
        // if (IS_LEFT_CHILD(new_node) && new_node->left == NULL)//bug
        // {
        //     head_->left = new_node;
        //     return;
        // }
        // if (IS_RIGHT_CHILD(new_node) && new_node->right == NULL)
        // {
        //     head_->right = new_node;
        //     return;
        // }
    }

public:
        void debug_in_trav(){
            debug_in_trav(getRoot());
            std::cout<<std::endl;
        }
        void debug_in_trav(node_ptr_type node){
            if (NULL == node)
            {
                return;
            }
            debug_in_trav(node->left);
            std::cout << ((node->data)) <<"("<< (node->height)<<") ";
            debug_in_trav(node->right);
        }


};

}
#endif
