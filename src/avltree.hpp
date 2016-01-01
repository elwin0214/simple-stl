/*
1.添加的节点是left 还是 right 不能确定 旋转类型
*/

#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <cmath>
#include "allocator.hpp"
#include "data_fill.hpp"

#define LEFT_SUB_HEIGHT(node) ((node)->left == NULL ? 0 : (node->left)->height)
#define RIGHT_SUB_HEIGHT(node) ((node)->right == NULL ? 0 : (node->right)->height)

namespace simple{

template <typename T>
struct TreeNode 
{

    TreeNode<T> *parent;
    TreeNode<T> *left;
    TreeNode<T> *right;
    T data;
    int height;
//private 声明 friend avltree  异常
public:
    TreeNode(const T& data):data(data),parent(NULL),left(NULL),right(NULL),height(1){

    }
} ;

template<typename T, typename Compare, typename Alloc = alloc>
class AVLTree
{
    public:
        typedef TreeNode<T>* node_ptr_type; 
        typedef TreeNode<T> node_value_type;
        typedef size_t size_type;
    private:
        typedef allocator<node_value_type,Alloc> _allocator;
        TreeNode<T> *root;
        TreeNode<T> *head;
        TreeNode<T> *tail;
        Compare compare;
        enum SubTreeType
        {
            DIRECT,
            LEFT,
            RIGHT
        };
    private:
        node_ptr_type allocate_and_fill(size_type n, const T& value) 
        {
            node_ptr_type result = _allocator::allocate(n);
            fill_n(result, n, value);
            return result;
        }

        void add_node_left(node_ptr_type node, const T &t)
        {
            node->left = allocate_and_fill(1, t);
            node->left->parent = node;
            int before_height = node->height;
            int cur_height = recount_height(node);
            if (cur_height == before_height) return; //无需旋转
            count_and_rotate(node);
        }

        void add_node_right(node_ptr_type node, const T &t)
        {
            node->right = allocate_and_fill(1, t);
            node->right->parent = node;
            int before_height = node->height;
            int cur_height = recount_height(node);
            if (cur_height == before_height) return; //无需旋转
            count_and_rotate(node);
        }

        void count_and_rotate(node_ptr_type node)
        {
            while (node != NULL)
            {
                int lh = LEFT_SUB_HEIGHT(node);//node->left == NULL ? 0 : node->left->height;
                int rh = RIGHT_SUB_HEIGHT(node);//node->right == NULL ? 0 : node->right->height;
                if (abs(lh - rh) >= 2)
                {
                    rotate(node, (lh > rh ? LEFT : RIGHT));
                    return;
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
            if (NULL == parent)
            {
                root = child;
            }
            else
            {
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
            /*当前旋转的节点node 是左子树还是右子树*/
            SubTreeType rotate_tree_type = DIRECT;
            if (NULL != node->parent)
            {
                rotate_tree_type = node->parent->left == node ? (LEFT) : (RIGHT); 
            }
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
                    // node_ptr_type parent = node->parent;
                    // node_ptr_type left = node->left;
                    // node_ptr_type left_right = left->right;

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

                    //set_to_rtree(right_left, parent);
                    //if (NULL == parent) root = right_left;
                    set_to_child(right_left, parent, rotate_tree_type);

                    recount_height(right_left->left);
                    recount_height(right_left->right);
                    recount_height(right_left);
                }
                else
                {
                    // node_ptr_type parent = node->parent;
                    // node_ptr_type right = node->right;
                    // node_ptr_type right_left = right->left;
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

         



    public:
        AVLTree():root(NULL),head(NULL),tail(NULL)
        {

        }
        node_ptr_type getRoot()
        {
            return root;
        }
        void insert(const T &t)
        {
            if (root == NULL)
            {
                root = allocate_and_fill(1, t);
            }
            else
            {
                node_ptr_type node = root;
                while (node != NULL)
                {

                    if (t == node->data)
                    {
                        //return node;
                    }
                    if (compare(t, node->data))
                    {
                        if (node->left != NULL) node = node->left;
                        else 
                        {
                            add_node_left(node, t);
                            return;
                        }
                    }
                    else
                    {
                        if (node->right != NULL) node = node->right;
                        else 
                        {
                            add_node_right(node, t);
                            return;
                        }
                        
                    }
                }
            }
        }
        #ifdef DEBUG
        void debug_in_trav(){
            debug_in_trav(root);
            std::cout<<std::endl;
        }
        void debug_in_trav(node_ptr_type node){
            if (NULL == node)
            {
                return;
            }
            debug_in_trav(node->left);
            std::cout << (node->data) <<"("<< (node->height)<<") ";
            debug_in_trav(node->right);
        }



        #endif

};
}
#endif
