// g++ constexpr.cpp -o constexpr -Wall -std=c++17

#include <iostream>
#include <cstdint>

// from c+=11
constexpr int gcd(int a, int b){
  return (b== 0) ? a : gcd(b, a % b);
}

// from c++14
constexpr auto gcd2(int a, int b) -> int {
  while (b != 0){
    auto tmp= b;
    b= a % b;
    a= tmp;
  }
  return a;
}

// A class with constexpr
// constructor and function
class Rectangle {
private:
    int _h, _w;
public:
    // A constexpr constructor
    constexpr Rectangle(int h, int w) : _h(h), _w(w) {} // constructed at compile time 
    constexpr int getArea() const { return (_h * _w); } // calc at compile time 
};



int main()
{
    int res2 = gcd2(135, 15);  // run time 
    constexpr int res = gcd(121, 11); // compile time
    std::cout << res2 << " , " << res << std::endl;
    
    constexpr Rectangle obj(10, 20);  
    constexpr int r = obj.getArea();
    std::cout <<  r <<  std::endl;
    
    return 0;
}
