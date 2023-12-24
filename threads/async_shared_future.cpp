#include <iostream>
#include <thread>
#include <functional>
#include <string>
#include <mutex>
#include <memory>
#include <future>
#include <iterator>
#include <algorithm>
#include <vector>

/* REMARK:  
    Just like thread and unique_lock, 
    future and promise can not be copied just moved
    std::promise p2 = std::move(p);
 */

class Factorial {
public:

    int operator()(std::shared_future<int> sf) {
        /* if the promise is not fulfilled
         f.get() will get exception std::future_errc::broken_promise */
        int n = sf.get();  // wait for promised value
        int res = 1;
        for(int i=n; i>0; i--)
            res*=i;
        return res;
    }

};



class AsyncSharedFuture {
    std::vector<int> vec;
public:
    using Func = std::function<int(std::shared_future<int>)> ;

    /* constructor that creates the thread */
    std::vector<int> factorial(Func func) {
        
         /* std::promise<T> is a promise to pass the thread a parameter in the future*/
        std::promise<int> p;
        std::future<int> f = p.get_future();
        std::shared_future<int> sf = f.share(); /* make f a shared future from promise (which can be copied) */

        /* what until fu.get() is called to lunch a thread 
           we pass future by ref because future can not be copied*/
        std::future<int> fu0 = std::async(std::launch::async,func, sf); // pass by value 
        std::future<int> fu1 = std::async(std::launch::async,func, sf);
        std::future<int> fu2 = std::async(std::launch::async,func, sf);

        /* do some stuff*/
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        p.set_value(9); // broadcast to all threads

        // or  
        // p.set_exception(std::make_exception_ptr(std::runtime_error("I am a idiot!!!")));
        vec.push_back(fu0.get());
        vec.push_back(fu1.get());
        vec.push_back(fu2.get());
        return vec;
    }
};




int main() {

    int n = 7;
    AsyncSharedFuture async_shared_fu;
    Factorial fact;

    std::vector<int> vec = async_shared_fu.factorial(fact);
    std::ostream_iterator<int> cout_it(std::cout, " ");
    std::copy(vec.begin(), vec.end(), cout_it);
    std::cout << "\n";
    return 0;
}