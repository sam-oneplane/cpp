/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <functional>


int f(int const x) 
{
    return 7+x;
}

struct S
{
    int x{10};
    int f(int const y)
    {
        return x+y;
    }
    int g(int const y)
    {
        return x*y;
    }
};

int main()
{
    /* example 1 */
    std::cout << std::invoke(&f, 20) << "\n";
    std::cout <<  "\n";
    S s;
    
    auto fp = &S::f ;
    int (S::*fg)(int) = &S::g ;
    
    std::cout << (s.*fp)(6) << "\n";
    std::cout << (s.*fg)(6) << "\n";
    std::cout <<  "\n";
    /* example 2 */
    int fk = std::invoke(&S::g, s, 9);
    std::cout << fk << "\n";
    fk = std::invoke(&S::f, s, 9);
    std::cout << fk << "\n";
    fk = std::invoke(&S::x, s);
    std::cout << fk << "\n";
    
    return 0;
}