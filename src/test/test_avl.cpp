#include <iostream>
#include <unistd.h>
#include <cpptest.h>
#include "../avltree.hpp"

#define LOG_DEBUG(fmt,...) fprintf(stdout, fmt, ##__VA_ARGS__)

using namespace std;
struct Compare
{
    bool operator ()(int a, int b)
    {
        return a < b;
    }
};
struct Data
{
    int key;
    int value;

    Data(int k, int v):key(k),value(v)
    {

    }
};
 
struct KeyOfValue
{
    //KeyOfValue(){}
    int operator ()(const Data &data)
    {return data.key;}
};


struct test_avltree : public Test::Suite
{
 
    void test_insert()
    { 
        KeyOfValue kov;
        simple::avltree<int, ::Data, KeyOfValue, Compare> tree;
        tree.insert(::Data(1, 1));
        tree.insert(::Data(2, 2));
        tree.insert(::Data(3, 3));
        tree.insert(::Data(4, 4));
    }


    void test_insert_ll()
    { 
        KeyOfValue kov;
        simple::avltree<int, ::Data, KeyOfValue, Compare> tree;
        for (int i = 3 ; i >=1; i--)
        {
            tree.insert(::Data(i, i));
        }
        simple::avltree<int, ::Data, KeyOfValue, Compare>::iterator itr = tree.begin();
        TEST_ASSERT( 1 == (itr->key));
    }

    void test_insert_lr()
    { 
        KeyOfValue kov;
        typedef simple::avltree<int, ::Data, KeyOfValue, Compare> AVLTree;
        AVLTree tree;

        tree.insert(::Data(3, 3));
        tree.insert(::Data(1, 1));
        tree.insert(::Data(2, 2));
        AVLTree::iterator itr = tree.begin();
        TEST_ASSERT( 1 == (itr->key));
        //tree.getRoot();
        AVLTree::node_ptr_type node_ptr = tree.getRoot();
        TEST_ASSERT( 2 == (node_ptr->data.key));
    }

    void test_insert_rr()
    { 
        KeyOfValue kov;
        typedef simple::avltree<int, ::Data, KeyOfValue, Compare> AVLTree;
        AVLTree tree;

        for (int i = 1 ; i <= 3; i++)
        {
            tree.insert(::Data(i, i));
        }
        AVLTree::iterator itr = tree.begin();
        TEST_ASSERT( 1 == (itr->key));
        AVLTree::node_ptr_type node_ptr = tree.getRoot();
        TEST_ASSERT( 2 == (node_ptr->data.key));
    }

    void test_insert_rl()
    { 
        KeyOfValue kov;
        typedef simple::avltree<int, ::Data, KeyOfValue, Compare> AVLTree;
        AVLTree tree;

        tree.insert(::Data(1, 1));
        tree.insert(::Data(3, 3));
        tree.insert(::Data(2, 2));
        AVLTree::iterator itr = tree.begin();
        TEST_ASSERT( 1 == (itr->key));
        AVLTree::node_ptr_type node_ptr = tree.getRoot();
        TEST_ASSERT( 2 == (node_ptr->data.key));
    }

    void test_find()
    { 
        KeyOfValue kov;
        typedef simple::avltree<int, ::Data, KeyOfValue, Compare> AVLTree;
        AVLTree tree;

        for (int i = 1 ; i <= 20; i++)
        {
            tree.insert(::Data(i, i));
        }
        AVLTree::iterator itr = tree.find(8);
        TEST_ASSERT( 8 == (itr->key));

        itr = tree.find(21);
        TEST_ASSERT(tree.end() == (itr));//not found
    }

    void test_insert_duplicate()
    { 
        KeyOfValue kov;
        typedef simple::avltree<int, ::Data, KeyOfValue, Compare> AVLTree;
        AVLTree tree;

        for (int i = 1 ; i <= 20; i++)
        {
            tree.insert(::Data(i, i));
        }
        tree.insert(::Data(8, 80));
        AVLTree::iterator itr = tree.find(8);
        TEST_ASSERT( 80 == (itr->value));
    }

    void test_erase()
    { 
        KeyOfValue kov;
        typedef simple::avltree<int, ::Data, KeyOfValue, Compare> AVLTree;
        AVLTree tree;

        for (int i = 1 ; i <= 20; i++)
        {
            tree.insert(::Data(i, i));
        }
         
        AVLTree::iterator itr = tree.find(1); //leaf node
        tree.erase(itr);
        itr =  tree.find(1);
        TEST_ASSERT(tree.end() == (itr));

        for (itr = tree.begin(); itr != tree.end(); itr++)
        {
            cout<<" " <<(itr->key)<<"-" <<(itr.node_ptr_->height) ;
        }
        cout<<endl;

        itr = tree.find(16); //no leaf node
        tree.erase(itr);
        itr =  tree.find(16);
        TEST_ASSERT(tree.end() == (itr));

        for (itr = tree.begin(); itr != tree.end(); itr++)
        {
            cout<<" " <<(itr->key)<<"-" <<(itr.node_ptr_->height) ;
        }
        cout<<endl;

        itr = tree.find(8); //root node
        tree.erase(itr);
        itr =  tree.find(8);
        TEST_ASSERT(tree.end() == (itr));

        for (itr = tree.begin(); itr != tree.end(); itr++)
        {
            cout<<" " <<(itr->key)<<"-" <<(itr.node_ptr_->height) ;
        }
        cout<<endl;   
        
    }

    test_avltree()
    {
        TEST_ADD(test_avltree::test_insert);
        TEST_ADD(test_avltree::test_insert_ll);
        TEST_ADD(test_avltree::test_insert_lr);
        TEST_ADD(test_avltree::test_insert_rr);
        TEST_ADD(test_avltree::test_insert_rl);
        TEST_ADD(test_avltree::test_find);
        TEST_ADD(test_avltree::test_insert_duplicate);
        TEST_ADD(test_avltree::test_erase);

  }
};

int main()
{
  test_avltree ta;
  Test::TextOutput output(Test::TextOutput::Verbose);
  (ta.run(output, false));
  return 0;
}