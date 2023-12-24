#include <iostream>
#include <thread>
#include <functional>
#include <string>
#include <memory>
#include <future>
#include <algorithm>
#include <vector>

class Factorial {
public:

    int operator()(std::future<int>& fu) {
        /* if the promise is not fulfilled
         f.get() will get exception ->  std::future_errc::broken_promise */
        int n = fu.get();  // from parent to child using promise
        int res = 1;
        for(int i=n; i>0; i--)
            res*=i;
        return res; // from child to parent thread using future
    }
};

class AsyncPromise {
public:
    using Func = std::function<int(std::future<int>&)> ;

    /* constructor that creates the thread running parent thread  */
    int factorial(Func func, int n) {
        
         /* std::promise<T> is a promise to pass the thread a parameter in the future*/
        std::promise<int> p;
        std::future<int> f = p.get_future();
        
        /* launch child thread */
        std::future<int> fu = std::async(std::launch::async, std::ref(func), std::ref(f));
        
        /* do some stuff*/
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        /* make the promise by setting future parameter */
        p.set_value(n);
        /* in case a promise is not made you can do : 
        p.set_exception(std::make_exception_ptr(std::runtime_error("I am an idiot!!!")));
        */

         std::this_thread::sleep_for(std::chrono::milliseconds(100));
         /* get future returned value */
        return fu.get();
    }
};


int main() {

    int n = 11;
    AsyncPromise async_p;
    Factorial fact;
    /* set the async thread */
    int res = async_p.factorial(fact, n);
    std::cout << "factorial of " <<  n << " is: " << res << std::endl;

    return 0;
}

