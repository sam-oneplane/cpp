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
#include <type_traits>
#include <limits>


template <typename T>
constexpr bool is_both()
{
    if constexpr(std::is_integral<T>::value && !std::is_same<bool, T>::value) 
    {
        if constexpr(std::numeric_limits<T>::max() < 1000)
        {
            return true;
        }
    }
    return false;
}

template <typename T>
auto print_type(T const &t)
{   // branch coosen at compile time using constexpr for diffrent types
    if constexpr(is_both<T>())
    {
        return t - 9 ;
    }
    else if constexpr(std::is_floating_point<T>::value)
    {
        return t - 0.3 ;
    }
    else
    {
        return t;
    }
}


int main()
{
    
    std::cout << print_type<int>(20) << "\n" ;
    std::cout << print_type<double>(5.5) << std::endl ;
    std::cout << print_type<bool>(true) << std::endl ;
    std::cout << print_type<std::string>("fuck of") << std::endl ;
    return 0;
}