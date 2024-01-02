#include <iostream>
#include <vector>
#include <iterator>
#include <forward_list>
#include <list>
#include <string>
using namespace std;

/* part of stl random access functions
advance(), distance(),  c++11 next(), prev() 
*/

/*
iterator categories in stl : pure abstraction (don't inhereite from base type)

1. input iterators
2. output iterators

STL container support
3. forward iterators  -> forward_list, un order asossiative containers
4. bi-directional iterators -> vector, list 
5. random access iterators -> vector, array , deque
*/

void print(string value) {
    operator<<(cout, value);
}

int main() {

    forward_list<int> col1 {1,2,4,9,6,8,10,12,15,18,21};
    vector<int> col2 {1,2,4,9,6,8,10,12,15,18,21};

    auto it1 = col1.begin();
    advance(it1, 4); // return void
    print(to_string(*it1)+"\n");

    auto it2 = col2.end();
    advance(it2, -3); // can do in a list or a vector
    print(to_string(*it2)+"\n");    

    it1 = col1.begin(); // c++11 return iterator
    auto it_next = next(it1, 5);
    print(to_string(*it_next)+"\n");

    it2 = col2.end();
    auto it_prev = prev(it2, 2);
    print(to_string(*it_prev)+"\n");

    auto dis = distance(col2.begin(), it_prev);
    print(to_string(dis)+"\n");
        

    return 0 ;
}

