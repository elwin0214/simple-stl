#ifndef __MALLOC__ALLOCATOR__
#define __MALLOC__ALLOCATOR__

#include <stddef.h>
#include <iostream>
#include <stdlib.h>

namespace simple
{

using namespace std;

class malloc_allocator
{

public:
  typedef size_t size_type;

  static void* allocate(size_type n) 
  {
    void* result = malloc(n);
    if (result == 0) 
    {
      cerr<<"out of memory"<<endl;
      exit(1);
    }
    return result;
  }
  static void* allocate() 
  {
     return allocate(1);
  }
  static void deallocate(void* p) 
  {
    free(p);
  }

  static void deallocate(void* p, size_t n) 
  {
    free(p);
  }
  
};

}

#endif