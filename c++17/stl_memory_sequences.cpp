// Type your code here, or load an example.
#include <string>
#include <vector>
#include <iostream>
#include <list>



int main()
{
    std::vector<std::vector<int>> vs;
    // c++11 uses rvalue move
    vs.push_back(std::vector<int>({1,2,3}));
    // or use a generated vector by calling vs constructor
    auto &p = vs.emplace_back(3,1); // in c++17
    // auto &p = vs.back(); in c++11
    p.push_back(4);

    std::list<int> l;
    auto &lp = l.emplace_front(5);
    std::cout << lp << "\n";
    // where this stay the same 
    std::list<int>::iterator i = l.emplace(l.begin(), 8);
    std::cout << *i << "\n";
    
    return p.size();
}