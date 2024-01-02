
#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <string>
#include <mutex>
#include <memory>

// this is the shared resource accessed by main thread and other threads
class LockLog {

    std::mutex m_mu;
    std::ofstream f;  //resource f is under protection of mutex

public:
    LockLog(const std::string path) {
        f.open(path);
    }
    // lock_guard acts as RAII on the mutex it is unlocking mutex when goes out of scope
    void shared_print(std::string msg, int id) {
        std::lock_guard<std::mutex> guard(m_mu);  // RAII  
        f << msg << id << std::endl;    // can throw exception
    }
    ~LockLog() {f.clear(); f.close();}
};

// RAII class thread with thread p1 with Functor and ref() to LockLog class  
class Thread {
    std::thread p1;
public:
    using Func = std::function<void(LockLog&)> ;
    /* constructor that creates the thread */
    Thread(Func func, LockLog& lock) {
        p1 = std::thread(func, std::ref(lock));
    }
    ~Thread() {
        p1.join();
        std::cout << "joining p1 and terminating Thread\n";
    }
};

// this is the thread functor with ref lock_log obj with mutex  
class Functor {
public:
    void operator()(LockLog &lock_log) {
        for (int i{0}; i<100; i++) {
            lock_log.shared_print( "functor thread : ", i);
        }
    }
};

// Using lambda function as an option to functor 
Thread::Func func2 = [](LockLog &lock_log) {
    for (int i{0}; i> -100; i--) {
        lock_log.shared_print( "functor2 thread : ", i);
    }
};

int main() {

    LockLog lock_log("./out/log.txt"); // shared obj with mutex 
    Functor func;
    Thread t1(func, lock_log);
    Thread t2(func2, lock_log);

    //std::this_thread::sleep_for(std::chrono::milliseconds(2));
    for (int i{0}; i<100; i++) {
        lock_log.shared_print( "main thread : ", i);
    }
   
    return 0;
}

//g++ -std=c++17  m_mutex.cpp  -o m_mutex 