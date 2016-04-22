#include "../allocator.hpp"
#include "../construct.hpp"
#include "../vector.hpp"
#include <cmath>    
#include "cpptest.h"
using namespace std;

class TestInt
{
private:
  int value;
public:
  TestInt(int value):value(value)
  {
    cout<<"TestInt() "<<value<<endl;

  }

  TestInt(const TestInt& ti):value(ti.value)
  {
  }

  ~TestInt()
  {
  }

  int getValue()
  {
    return value;
  }

  int getValue() const
  {
    return value;
  }
};

struct test_vector : public Test::Suite
{
  void test_op_arr()
  {
    TestInt testInt(2);
    simple::vector<TestInt> v(2,testInt);
    TEST_ASSERT(2 == v[0].getValue());
  }

  void test_push_back()
  {
    TestInt testInt(2);
    simple::vector<TestInt> v(2,testInt);
    TEST_ASSERT(2 == v.size());
    TEST_ASSERT(2 == v.capacity());
    v.push_back(3);
    TEST_ASSERT(3 == v.size());
    TEST_ASSERT(4 == v.capacity());

    v.push_back(4);
    v.push_back(5);
    TEST_ASSERT(5 == v.size());
    TEST_ASSERT(8 == v.capacity());
  }

  void test_empty_push_back()
  {
    TestInt testInt(2);
    simple::vector<TestInt> v;
    v.push_back(3);
    TEST_ASSERT(1 == v.size());
  }

  void test_iterator()
  {
     
    simple::vector<int> v;
    for (int i = 0; i < 100; i++)
      v.push_back(i);
    int index = 0;
    for (simple::vector<int>::iterator itr = v.begin();
      itr != v.end(); itr++)
    {
      TEST_ASSERT( (index++) == (*itr));
      //std::cout <<"test_iterator "<<(*itr) <<std::endl;
    }
  }

  void test_const_iterator()
  {
    TestInt testInt(2);
    simple::vector<TestInt> v(2,testInt);
    const simple::vector<TestInt> &const_v = v;
    v.push_back(3);
    for (simple::vector<TestInt>::const_iterator itr = const_v.begin();
      itr != const_v.end(); itr++)
    {
      std::cout <<"test_const_iterator "<<(itr->getValue()) <<std::endl;
    }
  }

  test_vector()
  {
    TEST_ADD(test_vector::test_op_arr);
    TEST_ADD(test_vector::test_push_back);
    TEST_ADD(test_vector::test_empty_push_back);
    TEST_ADD(test_vector::test_iterator);
    TEST_ADD(test_vector::test_const_iterator);  
  }

};

int main()
{
  test_vector tv;
  Test::TextOutput output(Test::TextOutput::Verbose);
  (tv.run(output, false));
  return 0;
}