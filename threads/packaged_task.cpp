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

/* REMARK:  
    Just like thread and unique_lock, future and promise can not be copied just moved
    std::promise p2 = std::move(p);
 */


int factorial(int n) {
    int res = 1;
    for(int i=n; i>0; i--)
        res*=i;
    return res;
}


int task_thread_1(int n) {
    /* template class which parametrized by func signature <int()> using std::bind()  */
    std::packaged_task<int(int)> t1(std::bind(factorial, n)); //  
    std::future<int> f1 = t1.get_future(); // get the task function value using future

    std::thread th1(std::move(t1), n);   // move task into thread
    
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    th1.join();
    /* get the value from the task function */
    return f1.get();

}

int task_thread_2(int n) {

    std::packaged_task<int()> t2(std::bind(factorial, n));
    std::future<int> f2 = t2.get_future();

    std::thread th2(std::move(t2)); 

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    th2.join();

    return f2.get();
}

int main() {

    int n = 12;
    std::cout << "task factorial with bind(" << n << ") in thread 1 = " << task_thread_1(n) << "\n";
    n = 7;
    std::cout << "task factorial with bind (" << n << ") in thread 2 = " << task_thread_2(n) << "\n";
    return 0;
}