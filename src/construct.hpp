#ifndef __CONSTRUCT__
#define __CONSTRUCT__

#include "traits.hpp"

namespace simple{

template <typename T>
void construct(T *ptr, const T &t) {
  new((void*)ptr) T(t);
}

template <typename T>
void destroy(T *ptr) {
  ptr->~T();
}

template <typename Iterator,typename T>
void __destroy(Iterator first, Iterator last, T* ) {
  for(; first < last; first++) {
    destroy(&*first);
  }
} 

template <typename Iterator>
void destroy(Iterator first, Iterator last) {
  __destroy(first, last, value_type(first)); 
}



}

#endif