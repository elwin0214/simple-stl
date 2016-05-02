/*
  平衡树，有个一个无不存储实际值的节点head_,该节点left、right分别指向最小、最大节点
  parent指向树的根节点。
  旋转方式见：http://blog.csdn.net/gabriel1026/article/details/6311339
*/

#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <cmath>
#include <iostream>
#include <stdio.h>
#include "allocator.hpp"
#include "data_fill.hpp"
#include "nocopyable.hpp"


#define LEFT_SUB_HEIGHT(node) ((node)->left == NULL ? 0 : (node->left)->height)
#define RIGHT_SUB_HEIGHT(node) ((node)->right == NULL ? 0 : (node->right)->height)

#define IS_ROOT_NODE(node) (head_->parent == (node))

#define IS_HEAD_NODE(node) (head_ == (node))
#define EXIT_ROOT() (head_->parent != NULL)

#define IS_FIRST_NODE(node) (head_->left == (node))
#define IS_LAST_NODE(node) (head_->right == (node))


#define IS_LEFT_CHILD(node)  ((head_ != (node)->parent) && ((node)->parent->left == (node)))
#define IS_RIGHT_CHILD(node)  ((head_ != (node)->parent) && ((node)->parent->right == (node)))


//#define LOG_DEBUG(fmt,...) fprintf(stdout, fmt, ##__VA_ARGS__)

namespace simple{

template <typename Value>
struct treenode 
{

    treenode<Value> *parent;
    treenode<Value> *left;
    treenode<Value> *right;
    Value data;
    int height;
public:
    treenode(const Value& data):data(data),parent(NULL),left(NULL),right(NULL),height(1){
        
    }
};

//iterator包含2个指针，一个指向当前节点，另外一个指向head_节点，便于迭代运算。
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
            {   
                #ifdef LOG_DEBUG
                LOG_DEBUG("wont go there");
                #endif
            }
        }
        node_ptr_ = node_ptr;
    }

  


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
class avltree : public nocopyable
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
    avltree():head_(alloc())
    {
        head_->left = head_;
        head_->right = head_;
        head_->parent = NULL;
    }

    ~avltree()
    {
        //不能类似于list迭代回收内存
        //因为迭代加，会走析构过的内存。
        if (EXIT_ROOT())
        {
            free(head_->parent);
        }
        dealloc(head_);
    }

    void free(node_ptr_type node_ptr)
    {
        if (NULL == node_ptr) return;
        node_ptr_type left = node_ptr->left;
        node_ptr_type right = node_ptr->right;
        destory_node(node_ptr);
        dealloc(node_ptr);
        free(left);
        free(right);
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
        if (!EXIT_ROOT())
        {
            #ifdef LOG_DEBUG
            LOG_DEBUG("insert, root, key=%d\n", key_of_value_(value));
            #endif
            node_ptr_type node_ptr = alloc_and_fill(value);
            node_ptr->parent = head_;

            head_->left = node_ptr;
            head_->right = node_ptr;
            head_->parent = node_ptr;
            return;
        }
        node_ptr_type node_ptr = getRoot();
        while (node_ptr != NULL)
        {
            bool value_less = compare_(key_of_value_(value), key_of_value_(node_ptr->data));
            bool data_less = compare_(key_of_value_(node_ptr->data), key_of_value_(value));
            if ( value_less && !data_less)
            {
                if (node_ptr->left != NULL) node_ptr = node_ptr->left;
                else 
                {
                    #ifdef LOG_DEBUG
                    LOG_DEBUG("insert, left, key=%d\n", key_of_value_(value));
                    #endif
                    node_ptr_type new_node = add_node_left(node_ptr, value);//更新head_->left
                    reset_head_children(new_node);
                    
                    return;
                }
            }
            else if (data_less && !value_less)
            {
                #ifdef LOG_DEBUG
                LOG_DEBUG("insert, right, key=%d\n", key_of_value_(value));
                #endif
                if (node_ptr->right != NULL) node_ptr = node_ptr->right;
                else
                {
                    node_ptr_type new_node = add_node_right(node_ptr, value);
                    
                    reset_head_children(new_node);
                    return;
                }
            }
            else if (!value_less && !data_less)
            {
                // height  take into account
                destory_node(node_ptr);
                fill_n(&(node_ptr->data), 1, value);
                return ;
            }
            else
            {
                return ;//the compare is defined in error.
            }

        }
            
    }

    iterator find(Key key)
    {
        if (!EXIT_ROOT())
        {
            return end();
        }
        node_ptr_type node_ptr = getRoot();
        while (node_ptr != NULL)
        {
            bool value_less = compare_(key, key_of_value_(node_ptr->data));
            bool data_less = compare_(key_of_value_(node_ptr->data), key);
            if ( value_less && !data_less)
            {
                if (node_ptr->left != NULL) node_ptr = node_ptr->left;
                else return end();
            }
            else if (data_less && !value_less)
            {
                
                if (node_ptr->right != NULL) node_ptr = node_ptr->right;
                else return end();
            }
            else if (!value_less && !data_less)
            {
                 
                return iterator(head_, node_ptr);
            }
            else
            {
                return end();//the compare is defined in error.
            }

        }
        return end();
    }
    /*删除节点分2种情况
      1.节点是叶节点，节点有一个子节点，直接删除，节点被子树替代，迭代寻找parent节点，计算height，进行旋转
      2.节点有左右子节点，用大于当前节点的最小节点（属于1的情况）进行交换，然后采取1的策略处理。
    */
    void erase(const iterator &itr)
    {

       node_ptr_type node_ptr= itr.node_ptr_;
       if (NULL == node_ptr || IS_HEAD_NODE(node_ptr)) return;
       if (node_ptr->left != NULL && node_ptr->right != NULL)
       {
            node_ptr_type closed_greater_ptr = get_most_left_node(node_ptr->right);//exist definitely
            swap_node(closed_greater_ptr, node_ptr);
       }
        node_ptr_type left = node_ptr->left;
        node_ptr_type right = node_ptr->right;
        node_ptr_type parent = node_ptr->parent;
        node_ptr_type existed_child = (NULL != left ? left : right);//only one or not exist
        if (IS_LEFT_CHILD(node_ptr))
        {
            parent->left = existed_child;
        }
        else
        {
            parent->right = existed_child;
        }
        if (NULL != existed_child) existed_child->parent = parent;
        destory_node(node_ptr);
        dealloc(node_ptr);
        count_and_rotate_after_remove(parent);
        reset_head_children(NULL);
    }

private:
    node_ptr_type alloc()
    {
        return _allocator::allocate(1);
    }

    void dealloc(node_ptr_type node_ptr)
    {
        _allocator::deallocate(node_ptr);
    }

    node_ptr_type alloc_and_fill(const Value& value) 
    {
        node_ptr_type result = _allocator::allocate(1);
        //construct((&(result->data)), value);//如果直接在成员data上构造，node里面的指针没有初始化，导致有bug
        fill_n(result, 1, value);
        return result;
    }

    void destory_node(node_ptr_type node_ptr)
    {
        destroy(&(node_ptr->data));
    }

    void swap_node(node_ptr_type &node_ptr_1, node_ptr_type &node_ptr_2)
    {
        std::swap(node_ptr_1->data, node_ptr_2->data);
        //std::swap(node_ptr_1->height, node_ptr_2->height);
        std::swap(node_ptr_1, node_ptr_2);
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
        node_ptr_type result = alloc_and_fill(t);
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
        node_ptr_type result = alloc_and_fill(t);
        node->right = result;
        node->right->parent = node;
        int before_height = node->height;
        int cur_height = recount_height(node);
        if (cur_height == before_height) return result; //无需旋转
        count_and_rotate(node);
        return result;
    }


    //删除与插入的旋转不同，删除时，height已经不准确，需要先计算再旋转。
    void count_and_rotate_after_remove(node_ptr_type node)
    {
        while (!IS_HEAD_NODE(node)) //node!=NULL
        {
            int lh = LEFT_SUB_HEIGHT(node);
            int rh = RIGHT_SUB_HEIGHT(node);
            node->height = std::max(lh, rh) + 1;
            if (abs(lh - rh) >= 2)
            {
                rotate(node, (lh > rh ? LEFT : RIGHT));//可能不需要一直循环下去
            }            
            node = node->parent;
        }
    }

    void count_and_rotate(node_ptr_type node)
    {
        while (!IS_HEAD_NODE(node)) //node!=NULL
        {
            int lh = LEFT_SUB_HEIGHT(node);
            int rh = RIGHT_SUB_HEIGHT(node);
            if (abs(lh - rh) >= 2)
            {
                rotate(node, (lh > rh ? LEFT : RIGHT));
                return;//only one node need to be rotate 
            }
            //int before_height = node->height;
            node->height = std::max(lh, rh) + 1;
            //if (before_height == node->height) return;
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
            #ifdef LOG_DEBUG
            LOG_DEBUG("set_to_child, root, key=%d\n", key_of_value_(child->data));
            #endif
        }
        else//根据当前是在parent节点下左子树还是右子树 进行替换
        {
            #ifdef LOG_DEBUG
            LOG_DEBUG("set_to_child, key=%d, parent=%d\n", key_of_value_(child->data), key_of_value_(parent->data));
            #endif

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
        #ifdef LOG_DEBUG
        LOG_DEBUG("rotate, key=%d\n", key_of_value_(node->data));
        #endif
        /*当前旋转的节点node 所处的子树是左子树还是右子树*/
        SubTreeType rotate_tree_type = DIRECT;
        //if (NULL != node->parent)//??
        //{
        //the parent node of this sub-tree should choose a new node as child after rotate.
        //rotate_tree_type is used to record which child we shuould take as.
        rotate_tree_type = node->parent->left == node ? (LEFT) : (RIGHT); 
        //}
        if (st == LEFT)//左子树高度大
        {
            node_ptr_type parent = node->parent;
            node_ptr_type left = node->left;
            node_ptr_type left_right = left->right;
            if (LEFT_SUB_HEIGHT(node->left) > RIGHT_SUB_HEIGHT(node->left)) //LL- 由子树的左子树决定
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

            if (LEFT_SUB_HEIGHT(node->right) > RIGHT_SUB_HEIGHT(node->right))//RR
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
       
    }


};

}
#endif
