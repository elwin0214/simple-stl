#include <iostream>
/**
  test trait
  get the information of target type of iterator
*/
using namespace std;
namespace test
{

struct CharIterator
{
    typedef char value_type;
};
struct IntIterator
{
    typedef int value_type;
};

struct true_pod {};
struct false_pod {};


void check(true_pod )
{
    cout<<"true pod" <<endl;
};

void check(false_pod )
{
    cout<<"false pod" <<endl;
};

template<typename T>
struct type_trait
{
    typedef false_pod pod_type;
};

template<typename T>
struct type_trait<T*>
{
    typedef true_pod pod_type;
};

//template<typename T>
template<>
struct type_trait<int>
{
    
    typedef true_pod pod_type;
};

template<typename Iterator>
struct iterator_trait
{
    typedef typename Iterator::value_type value_type  ;
};
//template<Iterator>
template<typename T>
struct iterator_trait<T*>
{
    typedef T value_type;
};


template<typename Iterator>
void check_pod_type1(Iterator iterator)
{
    typename iterator_trait<Iterator>::value_type type;
    //typename type_trait<t>::pod_type p;// error
};

template <typename Iterator>
typename iterator_trait<Iterator>::value_type* value_type_(const Iterator& itr)
{
    return (typename iterator_trait<Iterator>::value_type*)(0);
};

template<typename Iterator, typename T>
void check_pod_type_by_pointer(Iterator iterator, T* )
{
    
    typename type_trait<T>::pod_type is_pod;
    check(is_pod);
};

template<typename Iterator>
void check_pod_type2(Iterator iterator)
{
    //(typename iterator_trait<Iterator>::value_type*)(0)
    check_pod_type_by_pointer(iterator, value_type_(iterator));
};

}


int main()
{
    test::check_pod_type2(test::IntIterator());
    int *p = 0;
    test::check_pod_type2<int*>(p);
    test::check_pod_type2(test::CharIterator());
    
    
    return 0;
}