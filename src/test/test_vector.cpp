#include "../allocator.hpp"
#include "../construct.hpp"
#include "../vector.hpp"
#include <cmath>    
#include "cpptest.h"
using namespace std;

class TestInt{
private:
  int value;
public:
  TestInt(int value):value(value){}
  TestInt(const TestInt& ti):value(ti.value) {
  }
  ~TestInt(){
  }

  int getValue(){
    return value;
  }

  int getValue() const{
    cout <<"getValue() const" <<endl;
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
    v.push_back(3);
    TEST_ASSERT(3 == v.size());
  }

  void test_iterator()
  {
    TestInt testInt(2);
    simple::vector<TestInt> v(2,testInt);
    v.push_back(3);
    for (simple::vector<TestInt>::iterator itr = v.begin();
      itr != v.end(); itr++)
    {
      std::cout <<(itr->getValue()) <<std::endl;
    }

    //TEST_ASSERT( == v.size());
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
      std::cout <<(itr->getValue()) <<std::endl;
    }

    //TEST_ASSERT( == v.size());
  }
 

  test_vector()
  {
    TEST_ADD(test_vector::test_op_arr);
    TEST_ADD(test_vector::test_push_back);
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