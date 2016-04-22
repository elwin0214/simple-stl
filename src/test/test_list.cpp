#include "../list.hpp"
#include <iostream>
#include "cpptest.h"

using namespace std;

struct test_list : public Test::Suite
{
 
  void test_push_back()
  { 
    simple::list<int> lst;
    TEST_ASSERT( 0 == lst.size());
    lst.push_back(1);
    TEST_ASSERT( 1 == lst.size());
  }

  void test_iterator()
  {
    simple::list<int> lst;
    for (int i = 0; i < 100; i++)
    {
      lst.push_back(i);
    }
    simple::list<int>::iterator itr = lst.begin();
    int value = 0;
    for (; itr != lst.end(); itr++)
    {
      TEST_ASSERT(value++ == (*itr)); 
    }
  }

  void test_iterator_set()
  {
    simple::list<int> lst;
    for (int i = 0; i < 100; i++)
    {
      lst.push_back(i);
    }
    simple::list<int>::iterator itr = lst.begin();
    TEST_ASSERT(0 == (*itr)); 
    itr++;
    TEST_ASSERT(1 == (*itr)); 
    *itr = 100;
    TEST_ASSERT(100 == (*itr)); 
    
    const simple::list<int> &const_lst = lst;
    //simple::list<int>::const_iterator const_itr = lst.begin();//error
    //*const_itr = 100;

    //simple::list<int>::const_iterator const_itr = const_lst.begin();//error
    //*const_itr = 100;
  }


  void test_const_iterator()
  {
    simple::list<int> lst;
    for (int i = 0; i < 100; i++)
    {
      lst.push_back(i);
    }
    const simple::list<int> &const_lst = lst;
    simple::list<int>::const_iterator itr = const_lst.begin();
    int value = 0;
    for (; itr != const_lst.end(); itr++)
    {
      TEST_ASSERT(value++ == (*itr)); 
    }
  }
  
  test_list()
  {
    TEST_ADD(test_list::test_push_back);
    TEST_ADD(test_list::test_iterator);
    TEST_ADD(test_list::test_const_iterator);
    TEST_ADD(test_list::test_iterator_set);
  }
};

int main()
{
  test_list tl;
  Test::TextOutput output(Test::TextOutput::Verbose);
  (tl.run(output, false));
  return 0;
}