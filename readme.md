a toy stl imitate standard STL .

### environment
* g++
* cpptest

### feature

* vector：`push_back()`,`iterator`,`const iterator`   
* list：`push_back()`,`iterator`,`const iterator`   
* map：based on `AVL Tree`，support `insert()`,`find()`,`erase()`,`begin()`,`end()`   

### build

```
mkdir build
cd build
cmake ..
make test

```

### simple::map vs std::map

run `cd src/test; ./bench_test_map` after make.

```
case1 : size = 100000
avltree insert 100000 nodes spends 89ms
avltree find 100000 nodes spends 89ms
standard insert 100000 nodes spends 87ms
standard find 100000 nodes spends 87ms

case2 : size = 1000000
avltree insert 1000000 nodes spends 1060ms
avltree find 1000000 nodes spends 1060ms
standard insert 1000000 nodes spends 866ms
standard find 1000000 nodes spends 866ms

case3 : size = 10000000
avltree insert 10000000 nodes spends 12784ms
avltree find 10000000 nodes spends 12784ms
standard insert 10000000 nodes spends 9596ms
standard find 10000000 nodes spends 9596ms

```
