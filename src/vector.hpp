#ifndef __VECTOR__
#define __VECTOR__

#include <stddef.h>
#include "allocator.hpp"
#include "data_fill.hpp"


namespace simple
{

template <typename T, typename Alloc = alloc>
class vector
{
public:
  typedef T* iterator;
  typedef size_t size_type ;
  typedef T value_type; 

private:
  typedef allocator<T,Alloc> _allocator;
  iterator start;
  iterator finish;
  iterator end_itr;

protected:
  // iterator allocate_and_fill(size_type n, const T& value) 
  // {
  //   iterator result = _allocator::allocate(n);
  //   fill_n(result, n, value);
  //   return result;
  // }

iterator allocate_and_fill(size_type n, const T& value) 
{
    iterator result = _allocator::allocate(n);
    fill_n(result, n, value);
    return result;
}
void destroy_and_deallocate(iterator start, iterator end) 
{
    destroy(start, end);
    _allocator::deallocate(start);
}
void copy(iterator start, iterator end, iterator new_start)
{
    while (start != end)
    {
      fill_n(new_start, 1, *start);
      start++;
    }
}







public:
  vector():start(0),finish(0),end_itr(0) 
  {
  }

  vector(size_type n, const T& value) 
  {
    iterator itr=allocate_and_fill(n, value);
    start=itr;
    finish=itr+n;
    end_itr=finish;
  }

  ~vector() 
  {
    destroy_and_deallocate(start ,finish);
  }

  bool empty() 
  {
    return start==finish;
  }

  size_type size() 
  {
    return static_cast<size_type>(finish-start);
  }

  size_type capacity() 
  {
    return end_itr-start;
  }
  iterator begin()
  {
    return start;
  }
  iterator end()
  {
    return finish;
  }
  value_type& operator[](size_type n) 
  {
    return *(start+n);
  }

  void push_back(const T& value)
  {
    
    if (end_itr == finish)
    {
      size_type n = this->size();
      iterator new_start = _allocator::allocate(2 * n);
      copy(start, finish, new_start);
      destroy_and_deallocate(start, finish);
      start = new_start;
      finish = start + n;
      end_itr = start + 2 * n;
    }
    fill_n(finish++, 1, value);
  }


};

}
#endif