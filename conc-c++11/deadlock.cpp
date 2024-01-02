#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <string>
#include <mutex>
#include <memory>



class LockLog {

    std::mutex m_mu;
    std::mutex m_mu0;
    std::ofstream f;  //resource f is under protection of mutex

public:
    LockLog(const std::string path) {
        f.open(path);
    }
    void shared_print(std::string msg, int id) {
        /*  this part of code prevent deadlock by locking multiple mutex 
            where each guard adopts the mutex he needs 
        */
        std::lock(m_mu, m_mu0); // lock mutexes 
        std::lock_guard<std::mutex> guard(m_mu, std::adopt_lock);  // RAII unlock mutex when out of scope
        std::lock_guard<std::mutex> guard0(m_mu0, std::adopt_lock);  // RAII  
        std::cout << msg << id << std::endl;    // can throw exception
    }
    void shared_print0(std::string msg, int id) {
        std::lock(m_mu, m_mu0);
        std::lock_guard<std::mutex> guard(m_mu, std::adopt_lock);  // RAII
        std::lock_guard<std::mutex> guard0(m_mu0, std::adopt_lock);  // RAII  
        std::cout << msg << id << std::endl;    // can throw exception
    }
    ~LockLog() {f.clear(); f.close();}
};

class Thread {
    std::thread p1;
    static void f1(LockLog& lock_log) {
        for (int i{0}; i<100; i++) {
            lock_log.shared_print( "f1 thread : ", i);
        }
    }
public:
    /* constructor that creates the thread */
    Thread(LockLog& lock) {
        p1 = std::thread(Thread::f1, std::ref(lock));
    }
    ~Thread() {
        p1.join();
        std::cout << "joining p1 and terminating Thread\n";
    }

};

int main() {

    LockLog lock_log("out/log2.txt"); // shared obj with mutex 
    Thread t1(lock_log);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for (int i=0; i<100; i++) {
        lock_log.shared_print0( "main thread : ", i);
    }
   
    return 0;
}