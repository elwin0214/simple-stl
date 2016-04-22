#ifndef __SIMPLE_MAP__
#define __SIMPLE_MAP__


#include "algorithm/functional.hpp"
#include "avltree.hpp"

namespace simple{


template<typename T1, typename T2>
struct map_pair
{
    T1 first;
    T2 second;

    map_pair(const T1 &first, const T2 &second):first(first),second(second)
    {

    }

    map_pair(const map_pair<T1, T2> &pair):first(pair.first),second(pair.second)
    {
        
    }
    map_pair& operator=(const map_pair<T1, T2> &pair)
    {
        first = pair.first;
        second = pair.second;
        return *this;
    }

};


 

template<typename K, typename T, typename Compare = less<K>, typename Alloc = alloc>
class map
{
public:
    typedef map_pair<K, T> value_type;
    typedef K key_type;
    struct key_of_value{
        const K& operator()(const value_type &value)
        {
            return value.first;
        }
    };

private:
    typedef key_of_value key_of_value_;
    avltree<K, value_type, key_of_value_, Compare, Alloc> tree_;

public:
    typedef typename avltree<K, value_type, key_of_value_, Compare, Alloc>::iterator iterator;

public:
    void insert(value_type value)
    {
        tree_.insert(value);
    }

    void erase(const iterator &itr)
    {
        tree_.erase(itr);
    }

    iterator begin()
    {
        return tree_.begin();
    }

    iterator end()
    {
        return tree_.end();
    }

    iterator find(K k)
    {
        return tree_.find(k);
    }

};

}

#endif
