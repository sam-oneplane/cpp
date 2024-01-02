/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

namespace gen::vec::state
{
std::vector<int> gen_vec()
{
    return {1,2,3,4};
}
}


int main()
{
    auto V = gen::vec::state::gen_vec(); // we can do this type of namespacing
    auto const itr = std::find(V.begin(), V.end(), 2);
    if (itr != V.end())
    {
        *itr = 3;
    }
    
    // we can do this insted
    if ( auto const itr = std::find(V.rbegin(), V.rend(), 3); 
        itr != V.rend() )
    {
        *itr = 4;
    }
    
    for(auto it = V.begin(); it != V.end(); ++it) 
    {
        std::cout << *it << ", "; 
        int c = std::clamp(*it , 3, 10);  // *it > 10 -> max(*it, 3) else min(*it, 10)
        std::cout << c << "\n"; 
        
    }
    std::cout << "\n";
    
    return 0;
}