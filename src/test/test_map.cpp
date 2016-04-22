
#include "../map.hpp"
#include <iostream>
#include "cpptest.h"

using namespace std;

struct test_map : public Test::Suite
{
    void test_insert()
    { 
        typedef simple::map<int, int> smap;
        smap map;
        for (int i = 0; i < 10; i++ )
        {
            map.insert(smap::value_type(i, i));
        }
        

    }

    void test_iterator()
    { 
        typedef simple::map<int, int> smap;
        smap map;
        for (int i = 0; i < 10; i++ )
        {
            map.insert(smap::value_type(i, i));
        }
        int i = 0;
        for (smap::iterator itr = map.begin(); itr != map.end(); itr++)
        {
            TEST_ASSERT(i++ == (itr->first));
        }
    }

    void test_erase()
    { 
        typedef simple::map<int, int> smap;
        smap map;
        for (int i = 0; i < 10; i++ )
        {
            map.insert(smap::value_type(i, i));
        }

        smap::iterator itr = map.find(6);
        TEST_ASSERT(6 == (itr->first));
        
        map.erase(itr);
        itr = map.find(6);
        TEST_ASSERT(map.end() == (itr));
    }

    


    test_map()
    {
        TEST_ADD(test_map::test_insert);
        TEST_ADD(test_map::test_iterator);
        TEST_ADD(test_map::test_erase);
    }
};

int main()
{
  test_map tm;
  Test::TextOutput output(Test::TextOutput::Verbose);
  (tm.run(output, false));
  return 0;
}