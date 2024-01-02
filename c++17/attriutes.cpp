// Type your code here, or load an example.
#include <iostream>

struct [[nodiscard]] Error{}; // works on classes enums etc...
// you can't ignore return value 
[[nodiscard]]int foo() { return 5;}
Error goo() 
{
   Error e;
   return e; 
}
[[maybe_unused]] void zoo() {}



int main(int argc, [[maybe_unused]] char *arv[])
{
    int x{0};
    Error err;
    switch (argc)
    {
        case 0:
            x = foo();
            [[fallthrough]]; // instade of braking, fallthrough case 1
        case 1:
            err = goo();    
    }
    std::cout << x << "/n"; 
    return 0;
}