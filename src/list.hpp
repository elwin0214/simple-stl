#ifndef __SIMPLE_LIST__
#define __SIMPLE_LIST__

#include <stddef.h>
#include "allocator.hpp"
#include "data_fill.hpp"
#include "algorithm/base.hpp"
#include "nocopyable.hpp"
#include <iostream>

namespace simple
{
/**
  list node
*/
template <typename T>
struct _list_node
{
  _list_node<T> *before;
  _list_node<T> *after;
  T data;
  _list_node():before(NULL),after(NULL){}

};
/**
 list iterator
*/
template <typename T, typename Ref, typename Ptr>
struct _list_iterator 
{
  _list_node<T> *node_ptr_;
  typedef _list_node<T>* node_type_ptr;
  //typedef _list_iterator<T, T&, T*> iterator;
  typedef _list_iterator<T, const T&, const T*> const_iterator;
  typedef _list_iterator<T, Ref, Ptr> self;

  typedef T  value_type;
  typedef Ref reference;//it will return a const refrence if const_iterator
  typedef Ptr pointer;

public:
  _list_iterator(node_type_ptr node_ptr):node_ptr_(node_ptr)
  {

  }

  self& operator++() 
  {
    node_ptr_ = node_ptr_->after;
    return *this;
  }

  self operator++(int) 
  {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--() 
  {
    node_ptr_ = node_ptr_->before;
    return *this;
  }

  self operator--(int) 
  {
    self tmp = *this;
    --*this;
    return tmp;
  }

  reference operator*() 
  {
    return (*node_ptr_).data;
  }

  pointer operator->() 
  {
    return &(operator*());
  }

  bool operator==(const self &itr)  //self is iterator or const_iterator
  {
    return node_ptr_ == itr.node_ptr_;
  }

  bool operator!=(const self &itr) 
  {
    return node_ptr_ != itr.node_ptr_;
  }

};


template <typename T, typename Alloc = alloc>
class list : public nocopyable
{

private:
  typedef _list_node<T> list_node;
  typedef allocator<list_node,Alloc> _allocator;


public:
  typedef _list_iterator<T, T&, T*> iterator;
  typedef _list_iterator<T, const T&, const T*> const_iterator;

  typedef list_node* node_type;
  typedef size_t size_type;
  typedef T& refrence;
  typedef const T& const_refrence;

protected:
  node_type head_;

  node_type create()
  {
    return _allocator::allocate();
  }

  node_type create_and_fill(const T& value)
  {
    node_type node = _allocator::allocate();
    construct(&(node->data), value);
    return node;
  }

  void destory_node(node_type node)
  {
    destroy(&(node->data));
    _allocator::deallocate(node);
  }

public:
  list():head_(create())
  {
    //node_ptr = get_node();
    head_->before = head_;
    head_->after = head_;
  }

  iterator begin() 
  {
    return head_->after;
  }

  const_iterator begin() const
  {
    return head_->after;
  }

  iterator end()
  {
    return head_;
  }

  const_iterator end() const
  {
    return head_;
  }

  refrence front()
  {
    return (*begin());
  }

  const_refrence front() const
  {
    return (*begin());
  }
  refrence back()
  {
    return (*(end() - 1));
  }

  const_refrence back() const
  {
    return (*(end() - 1));
  }


  void push_back(const T& value)
  {
    node_type node = create_and_fill(value);
    node_type tail = head_->before;
    tail->after = node;
    node->before = tail;
    node->after = head_;
    head_->before = node;

  }

  size_t size() const
  {
    return distance(begin(), end());
  }

  ~list()
  {
    iterator itr = begin();
    iterator end_itr = end();
    for (; itr != end_itr; )
    {
      iterator tmp = itr;
      ++itr;
      destory_node(tmp.node_ptr_);
    }
    _allocator::deallocate(head_);
  }

};

}
#endif











