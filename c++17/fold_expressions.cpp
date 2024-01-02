/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>


template <typename ... T> // parameter pack
auto sum(T ... t)         // unpack
{
    typename std::common_type<T ...>::type result{};
    (void)std::initializer_list<int>{ (result += t, 0) ... };
    return result;
}

// in c++17 we can do 
template <typename ... T> // parameter pack
auto sum2(T ... t)         // unpack
{
    return (t + ...);    
}

template <typename ... T> // parameter pack
auto f_div(T ... t)         // unpack
{
    return (t / ...);    
}

template <typename ... T> // parameter pack
auto f_div2(T ... t)         // unpack
{
    constexpr int n{5};
    return (n / ... / t);    
}

template <typename ... T> // parameter pack
auto avg(T ... t)         // unpack
{
    return (t + ...) / sizeof...(t);    
}

int main()
{
    std::cout << sum(1,3.1,5,7.5,0.9,12) << "\n";
    std::cout << sum2(1,3.1,5,7.5,0.9,12) << "\n";
    std::cout << f_div(1,3.1,5) << "\n";
    std::cout << f_div2(1,3.1,5) << "\n";
    std::cout << avg(1,3.1,5,7.5,0.9,12) << "\n";
    return 0;
}