#ifndef __SIMPLE_ALG_BASE__
#define __SIMPLE_ALG_BASE__

#include <stddef.h>

namespace simple
{

template<typename InputIterator>
size_t distance(InputIterator start, InputIterator end)
{
    size_t dis = 0;
    for(; start != end; ++start)
        dis++;
    return dis;
}



}

#endif