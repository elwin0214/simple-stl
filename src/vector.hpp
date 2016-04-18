#ifndef __SIMPLE_VECTOR__
#define __SIMPLE_VECTOR__

#include <stddef.h>
#include "allocator.hpp"
#include "data_fill.hpp"


namespace simple
{

template <typename T, typename Alloc = alloc>
class vector
{
public:
    //typedef T* iterator;  
  typedef size_t size_type ;
  typedef T value_type; 
  typedef value_type* pointer;
  typedef value_type* iterator;
  typedef value_type& refrence;
  typedef const value_type* const_iterator;

private:
  typedef allocator<T,Alloc> _allocator;
  iterator start_;
  iterator pos_;
  iterator end_;

protected:

  iterator allocate_and_fill(size_type n, const T& value) 
  {
    iterator result = _allocator::allocate(n);
    fill_n(result, n, value);
    return result;
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
  vector():start_(0),pos_(0),end_(0) 
  {
  }

  vector(size_type n, const T& value) 
  {
        iterator itr=allocate_and_fill(n, value);
        start_ = itr;
        pos_ = itr+n;
        end_ = pos_;
  }

    ~vector() 
    {
      if (start_ > 0)
      {
        destroy(start_ ,pos_);
        _allocator::deallocate(start_);
      }
    }

  bool empty() 
  {
    return start_ == pos_;
  }

  size_type size() const
  {
    return static_cast<size_type>(pos_ - start_);
  }

  size_type capacity() const
  {
    return end_ - start_;
  }

  iterator begin() 
  {
    return start_;
  }

  const_iterator begin() const 
  {
    return start_;
  }

  iterator end()
  {
    return pos_;
  }

  const_iterator end() const 
  {
    return pos_;
  }

  refrence operator[](size_type n) 
  {
    return *(start_ + n);
  }

  refrence front()
  {
    return *begin();
  }

  refrence back()
  {
    return *(end() - 1);
  }

  void push_back(const T& value)
  {
    
    if (end_ == pos_)
    {
      size_type n = this->size();
      iterator new_start = _allocator::allocate(2 * n);
      copy(start_, pos_, new_start);
      //destroy_and_deallocate(start_, pos_);
      destroy(start_ ,pos_);
      _allocator::deallocate(start_);
      start_ = new_start;
      pos_ = start_ + n;
      end_ = start_ + 2 * n;
    }
    fill_n(pos_++, 1, value);
  }

  iterator data()
  {
    return start_;
  }

};

}
#endif