
#include <iostream>
#include <chrono>
#include <deque>
#include <thread>


typedef struct {
    unsigned int bytes;
    std::chrono::_V2::system_clock::time_point t;
} ByteStamp;

class DataFrame {
    std::deque<ByteStamp> qstamp;
    unsigned int totalBytes=0;
public:
    DataFrame() = default;

    void receiveBytes(const unsigned int bytes) {

        ByteStamp bs = {bytes, std::chrono::system_clock::now()};
        totalBytes += bytes;
        qstamp.push_back(std::move(bs));
    }

    /*  send bytes on demand using time-stamp
        where the delta time is one second*/
    int sendBytes() {
        auto ts = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds;

        if(qstamp.size() == 0)
            return 0;
        // return const_iterator
        auto it = qstamp.cbegin();
        for (; it < qstamp.cend(); ++it) {
            elapsed_seconds = ts - it->t;
            std::cout << "delta t = " << elapsed_seconds.count() << "\n";
            // when the elapsed_time is below 1 sec stop dec bytes 
            // erase the beging up to the it so you left with the newest bytes entered
            // with in 1 sec time frame 
            if (elapsed_seconds.count() <= 1.0) {
                qstamp.erase(qstamp.begin(), it);
                break;
            }
            // remove the bytes that the elapsed_time is older then 1 sec  
            totalBytes -= it->bytes ;
        }
        
        return totalBytes;
    }

    int getTotal() {
        return totalBytes;
    }

    int getSize() {
        return qstamp.size();
    }
};

std::ostream& operator<<(std::ostream& os, DataFrame& df) {
    return os << df.sendBytes() << " Bytes Sent,  deque has " << df.getSize() << " objects\n"; 
 }

void print(DataFrame &df) {
    std::cout << df ;
}

int main() {

    DataFrame df;
    for (int i=0; i<4; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        df.receiveBytes(10*(i+1));
    }
    std::cout << "Total bytes " << df.getTotal() << " from " << df.getSize() <<  " objects" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    print(df);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    df.receiveBytes(80);
    std::cout << "Total bytes " << df.getTotal() << " from " << df.getSize() <<  " objects" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(350));
    print(df);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));    
    print(df);

    return 0;
}
