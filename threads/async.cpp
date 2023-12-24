#include <iostream>
#include <thread>
#include <functional>
#include <string>
#include <memory>
#include <future>
#include <algorithm>
#include <vector>


/* REMARK:  
    Just like thread and unique_lock, 
    future and promise can not be copied just moved
    std::promise p2 = std::move(p);
 */

class Factorial {
public:
    int operator()(int n) {
        int res = 1;
        for(int i=n; i>0; i--)
            res*=i;
        return res;
    }

    void run(int n) {
        int res = 1;
        for(int i=n; i>0; i--)
            res*=i;
        std::cout << "Factorial::run(" << n << ") = " << res << std::endl;
    }
};


class Async {
public:
    using Func = std::function<int(int)> ;  // Factorial functor 
    /* create the thread using async returns a future */
    int factorial(Func fact, int data) {
        /* async function using a channel future to return value from child thread 
        the future object get back res in the future as use get() only once to return the value 
        std::launch::deferred - does not create a thread only postpone the call to fact
        std::launch::async - create a chld thread  
        */
        std::future<int> fu = std::async(std::launch::deferred | std::launch::async, fact, data);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        /* get value from future thread - only once */
        return fu.get();
    }
};

int main() {

    int n = 7;
    Async async;
    Factorial func;

    int res0 = async.factorial(func, n);
    std::cout << "factorial with functor(" << n << ") is " << res0 << std::endl;
    n = 10;

    /* async with lambda inst. of Functor */
    int res1 = async.factorial([](int n)
    {
        int r=1;
        for(int i{n}; i>0; i--)
            r*=i;
        return r;
    }, n);
    /* lunch a thread with class function */
    std::thread t1(&Factorial::run, &func, n+1);
    /* do some stuff*/
    std::this_thread::sleep_for(std::chrono::milliseconds(350));
    
    std::cout << "factorial with lambda(" << n << ") is " << res1 << std::endl;
    
}