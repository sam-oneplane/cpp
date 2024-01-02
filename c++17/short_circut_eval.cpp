/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>

using namespace std;

struct S{
  int x;
  int y;
  int z;
};

int main()
{
    auto [i, j, k] = S{0, 0, 3};
    int res1 = j  || k;
    std::cout << res1 << std::endl; 
    return 0;
}
