#include <iostream>
#include <thread>
#include <memory>
#include <future>
#include <iterator>
#include <algorithm>
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>



int factorial(int n) {
    int res = 1;
    for(int i=n; i>0; i--)
        res*=i;
    return res;
}

int main() {

    std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now() + 
                                                    std::chrono::milliseconds(2);
    /* THREADS*/
    std::thread t1(factorial, 7);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::this_thread::sleep_until(tp);
    

    /* MUTEX */
    std::mutex mu ;
    std::lock_guard<std::mutex> guard(mu);
    std::unique_lock<std::mutex> locker(mu);   
    locker.lock(); 
    locker.unlock();
    locker.try_lock(); // try if not succeed return
    locker.try_lock_for(std::chrono::milliseconds(100));
    locker.try_lock_until(tp);


    /* CONDITION VARIABLE */
    /*  condition variable works only with unique locks 
        because it need to be able to lock and unlock the mutex many times
        and lock_guard can lock unlock only once
     */
    std::condition_variable cond;
    cond.wait(locker, [](){});// some condition on shared resource like : is_empty() ;
    cond.wait_for(locker, std::chrono::milliseconds(100)) ; 
    cond.wait_until(locker, tp);


    /* FUTURE AND PROMISE objects*/
    /*  3 ways to get a future 
    1- promise::get_future()
    2- packaged_task::get_future()
    3- async() returns a future 
*/
    std::promise<int> p;
    std::future<int> fu = p.get_future();

    fu.get();  // retrieve the value
    fu.wait();
    fu.wait_for(std::chrono::milliseconds(100));
    fu.wait_until(tp);
    // later on
    p.set_value(7);

    
    /* async() function */
    std::future<int> fu = std::async(factorial, 7);


    /* PACKAGE_TASK */
    std::packaged_task<int(int)> task(factorial);
    std::future<int> fu = task.get_future();
    //  
    task(7);
    // in main thread 
    fu.get();


    return 0;
}