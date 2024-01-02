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

std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;

int factorial(int n) {
    int res = 1;
    for(int i=n; i>0; i--)
        res*=i;
    return res;
}

void push(int n, std::packaged_task<int()> &t) {
    std::unique_lock<std::mutex> locker(mu);
    task_q.push_back(std::move(t));
}

void pop(std::packaged_task<int()> &task) {
    std::unique_lock<std::mutex> locker(mu);
    cond.wait(locker, [](){return !task_q.empty();}); // condition free mutex if task_q is empty
    task = std::move(task_q.front()); // move from task_q into task
    task_q.pop_front();
}

void task_thread_q() {
    std::packaged_task<int()> task ;
    pop(task);
    task(); // after pop a task, run
}

int task_bind_with_thread(int n) {

    std::thread t(task_thread_q);
    // package_task links a callable obj to a future, important in threads 
    std::packaged_task<int()> task(std::bind(factorial, n)); 
    std::future<int> fu = task.get_future();
    push(n, task);
    cond.notify_one(); // notify mutex to lock if he can
    

    int task_r = fu.get(); // future get back the result from thread 
    t.join();
    
    return task_r;
}

int main() {

    for (int i {1}; i < 10; i++) 
    {
        std::cout << "Task " << i << "Returns : " << task_bind_with_thread(i) << "\n";
    }
}