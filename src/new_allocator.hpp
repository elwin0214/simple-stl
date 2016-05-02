#ifndef __SIMPLE_NEW_ALLOCATOR__
#define __SIMPLE_NEW_ALLOCATOR__

#include <stddef.h>
#include <iostream>

namespace simple
{

using namespace std;

template <typename T>
class new_allocator
{

public:
  typedef T* pointer;
  typedef size_t size_type;

  pointer allocate(size_type n) 
  {
    set_new_handler(0);
    T* tmp=(T*)(::operator new (size * sizeof(T)));
    if (tmp == 0) 
    {
      cerr<<"out of memory"<<endl;
      exit(1);
    }
    return tmp;
  }
  void deallocate(pointer p) 
  {
    ::operator delete(ptr);
  }

};
}

#endif