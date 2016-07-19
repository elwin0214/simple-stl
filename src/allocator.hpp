#ifndef __ALLOCATOR__
#define __ALLOCATOR__
#include "malloc_allocator.hpp"

namespace simple {

typedef malloc_allocator alloc;

template<typename T,typename Alloc>
class allocator
{
public:
  static T* allocate(size_t n)
  {
    return (0 == n ? 0 : (T*) Alloc::allocate(n * sizeof(T)));
  }

  static T* allocate()
  {
    return (T*) Alloc::allocate(sizeof(T));
  }

  static void deallocate(T *p) 
  {
    Alloc::deallocate(p,sizeof(T));
  }

};

}
#endif