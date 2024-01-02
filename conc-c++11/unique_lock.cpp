#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <string>
#include <mutex>
#include <memory>

class LockLog {

    std::mutex m_mu;
    std::once_flag m_flag;
    std::ofstream f;  //resource f is under protection of mutex
    std::string m_path;

public:
    LockLog(const std::string path): m_path(path) {}
    void shared_print(std::string msg, int id) {
        std::unique_lock<std::mutex> locker(m_mu, std::defer_lock); // RAII
        
        /*  initialization upon use Idiom 
            to avoid open/close mutex every time we want to
            check that the file is open we use call_once and 
            by single thread
        */
        std::call_once(m_flag, [&](){f.open(m_path);});
       
        locker.lock();
        f << msg << id << std::endl; // can throw exception
        locker.unlock();
        /* 
        1. you can lock and unlock again
        locker.lock();
        locker.unlock();

        2. you can move unique_lock
        std::unique_lock<std::mutex> locker2 = std::move(locker);
        */
    }

    ~LockLog() {f.clear(); f.close();}
};

class Thread {
    std::thread p1;
    /* NOT SO GOOD EXAMPLE  : using static func */ 
    static void f1(LockLog& lock_log) {
        for (int i{0}; i>-100; i--) {
            lock_log.shared_print( "f1 thread : ", i);
        }
    }
public:
    using Func = std::function<void(LockLog&)> ;
    /* constructor that creates the thread */
    Thread(Func func, LockLog& lock) {
         p1 = std::thread(func, std::ref(lock));
    }
    ~Thread() {
        p1.join();
        std::cout << "joining p1 and terminating Thread \n";
    }
};

class Functor {
public:
    void operator()(LockLog &lock_log) {
        for (int i{0}; i> -100; i--) {
            lock_log.shared_print( "functor thread : ", i);
        }
    }
};

int main() {
    Functor f;
    LockLog lock_log("out/log3.txt"); // shared obj with mutex 
    Thread t1(f, lock_log);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for (int i{0}; i<100; i++) {
        lock_log.shared_print( "main thread : ", i);
    }
    return 0;
}
