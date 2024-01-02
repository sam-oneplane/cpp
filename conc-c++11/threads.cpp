#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <string>


void f() {
    int counter = 100;
    while (counter > 0)
        counter--;
    
    std::cout << "running from thread\n";
}

void func1() {
    std::thread t1(f); /* thread t1 created f() starts running */
    std::cout << "running from main thread\n"; /* main thread control the resource of ostream */
    std::this_thread::sleep_for(std::chrono::milliseconds(100));


    /* join or detach can be done once*/
    t1.detach();    /* t1 becomes a deamon thread - c++ run time lib will end the treads life */
    if (t1.joinable())
        t1.join();  /* main thread waits from t1 thread to finish */
}

void func2() {
    std::thread t1(f); /* thread t1 created f() starts running */
    try {
        /* code */
        std::cout << "running from main thread\n"; /* main thread control the resource of ostream */
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    catch(const std::exception& e) {
        t1.join();
        throw e;
        std::cerr << e.what() << '\n';
    }
    t1.join();

}

class Functor {
public:
    void operator()(std::string msg) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << std::this_thread::get_id() << " says : " << msg << std::endl;
    }
};


class Thread {
    std::thread p1;
    std::string m_str;
public:
    using Func = std::function<void(std::string)> ;
    Thread(std::string msg) : m_str(msg) {}
    void run(Func func) {
        /*  parameter to a thread always past by value.
            to pass by ref do std::ref() or std::move() or use c pointers
        */
        //p1 = std::thread(func, std::ref(m_str));
        p1 = std::thread(func, std::move(m_str));
    }
    ~Thread() {
        std::cout << "joining p1 and terminating Thread\n";
        p1.join();
    }
};

int main() {

    //func1();
    //func2();
    
    Functor functor;
    std::string msg = "fuck you all suckers!";
    Thread t3(msg);
    t3.run(functor);
    std::cout << "running from main thread\n"; /* main thread control the resource of ostream */
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    /* to avoid oversubscription and context switching use : */
    std::cout << " num of threads : " << std::thread::hardware_concurrency() << std::endl;

    return 0;
}
