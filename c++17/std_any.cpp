/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <any>
#include <vector>
#include <string>
#include <type_traits>

std::string operator ""_s(const char* str, size_t)
{
    return std::string(str);
};

struct S 
{
  S() = default;
  S(S const &s) = default;
  int val{6} ;
};

int main()
{
    // check if s is a nothrow move is_nothrow_move_constructible
    static_assert(std::is_nothrow_move_constructible<S>::value, "no-throw");
    
    std::vector<std::any> V {5, 3.15 , "fuck_you"_s, S()};

    
    std::cout << V.size() << "\n";
    std::cout << std::any_cast<int>(V[0]) << "\n" ;
    std::cout << std::any_cast<S>(V[3]).val << "\n" ;
    std::cout << sizeof(std::any) << "\n" ; 
    
    double *p = std::any_cast<double>(&V[1]);
    *p = 5.25;
    std::cout << std::any_cast<double>(V[1]) << "\n" ;
    
    // any object <= 16 bytes,  which is move_constructible will not demand dynamic alloc
    return 0;
}