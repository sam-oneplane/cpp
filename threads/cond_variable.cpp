#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <string>
#include <mutex>
#include <memory>
#include <deque>
#include <condition_variable>

/*  condition variable works only with unique locks 
    because it need to be able to lock and unlock the mutex many times
    and lock_guard can lock unlock only once */

std::deque<int> q;
std::mutex mu;
std::condition_variable cond; 

class Thread {
    std::thread p;
public:
    using Func = std::function<void(int)> ;
    /* constructor that creates the thread */
    Thread(Func func, int data) {
        p = std::thread(func, data);
    }
    ~Thread() {
        p.join();
        std::cout << q.empty() << std::endl;
        std::cout << "joining main and terminating Thread\n";
    }
};

class Producer {
public:
    void operator()(int count) {
        this->c = count;
        while (c > 0) {
            std::unique_lock<std::mutex> locker(mu);
            q.push_back(c);
            locker.unlock();
            cond.notify_one(); // cond.notify_all()
             // wake up thread that wait for the condition
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            c--;
        }
    }
private:
    int c{0};
};

class Consumer {
public:
    void operator()(int data ) {
        this->flag = data;
        while (flag != 1) {
            /* use unique_lock because cond lock and unlock many times*/ 
            std::unique_lock<std::mutex> locker(mu);
            cond.wait(locker, [](){return !q.empty();}); /* wait until condition meet by predicate */
             /* unlock the unique_lock when waiting, 
                lock again when activated
                check if condition in lambda is meet. */ 
            flag = q.back();
            q.pop_back();
            locker.unlock();
            std::cout << "consumer thread got the value : " << flag << std::endl;
        }
    }
private:
    int flag{0};
};


int main() {
    Consumer consumer;
    Producer producer;
    Thread t2(consumer, 0); // temp Consumer moved to thread t2
    Thread t1(producer, 15); // 
 
    return 0;
}