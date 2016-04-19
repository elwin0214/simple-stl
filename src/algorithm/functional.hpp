#ifndef __SIMPLE_FUNCTIONAL__
#define __SIMPLE_FUNCTIONAL__

namespace simple
{

template<typename T>
struct less
{

    bool operator()(const T &l, const T &r)
    {
        return l < r;
    }
};

}

#endif
