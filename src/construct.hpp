#ifndef __SIMPLE_CONSTRUCT__
#define __SIMPLE_CONSTRUCT__

#include "traits.hpp"

namespace simple{

template <typename T>
void construct(T *ptr, const T &t) {
    new((void*)ptr) T(t);
}

template <typename N,typename T>
void construct(N *ptr, const T &t) {
    new((void*)ptr) N(t);
}

//
template <typename Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator& itr)
{
    return (typename iterator_traits<Iterator>::value_type*)(0);
}


template <typename T>
void destroy(T *ptr) {
    ptr->~T();
}

//萃取对象类型，如果是pod，无须调用析构函数
template <typename Iterator>
void __destroy(Iterator first, Iterator last, const __false_pod_type__ &) {

} 

template <typename Iterator>
void __destroy(Iterator first, Iterator last, const __true_pod_type__ &) {
    for(; first < last; first++) {
        destroy(&*first);
    }
} 

template <typename Iterator,typename T>
void __destroy(Iterator first, Iterator last, T* ) {
    // for(; first < last; first++) {
    //     destroy(&*first);
    // }
    typename type_traits<T>::is_pod_type pod_type;
    __destroy(first, last, pod_type);
} 

template <typename Iterator>
void destroy(Iterator first, Iterator last) {
    __destroy(first, last, value_type(first)); 
}



}

#endif