#ifndef __SIMPLE_TRAITS__
#define __SIMPLE_TRAITS__

namespace simple 
{

struct __true_pod_type__{};
struct __false_pod_type__ {};

template <typename Iterator>
struct type_traits 
{
    typedef __false_pod_type__ is_pod_type;
};


template <typename T>
struct type_traits<T*>//pointer type
{
    typedef __true_pod_type__ is_pod_type;
};

template <>
struct type_traits<int>//int type
{
    typedef __true_pod_type__ is_pod_type;
};





}

#endif