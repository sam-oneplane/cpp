#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
using namespace std;


int main() {

    // 1. Stream iterator take a in/out stream and iterate over it like it was a collection
    vector<int> col;

    /* ITERATORS */
    istream_iterator<int> eos;/*input stream iterator will declare eos when he reads a non int or eof*/
    istream_iterator<int> cin_it(cin); // default construct istream cin 
    ostream_iterator<int> cout_it(cout, " ");
    auto back_it = back_inserter(col);

    /*
    for (; cin_it != eos ; ++cin_it) {
        //col.push_back(*cin_it); the normal way to push to vector
        *back_it = *cin_it; //2. use insert iterator to push elements to col
    }
    //3. reverse iterator for all reversable collactions
    for(auto it = col.rbegin(); it != col.rend(); ++it) {
        *cout_it = *it;  // iterators are pointers
    }
    cout << "\n";
    */

    // using copy 
    copy(cin_it, eos, back_it); // use copy to back iterate on vector
    copy(col.rbegin(), col.rend(), cout_it); 
    cout << "\n";

    return 0;
}

