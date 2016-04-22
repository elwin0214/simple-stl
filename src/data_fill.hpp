#ifndef __DATA__FILL__
#define __DATA__FILL__

#include "construct.hpp"

namespace simple {

template <typename Iterator, typename Size, typename T>
void _fill_n(Iterator itr, Size n, const T& value)
{
  Iterator start=itr;
  for(; n>0; n--, start++) 
  {
    construct(&*start, value);
  }
}

template <typename Iterator, typename Size, typename T>
void fill_n(Iterator itr, Size n, const T& value) 
{
  _fill_n(itr, n, value);
}


}
#endif