#include <vector>
#include <iostream>


class Vec {

    int size_ ;
    int* ptr_ ;
    

public:

    Vec() = default;
    Vec(const int size, int* ptr) : size_(size), ptr_(ptr) {} 
    Vec(const Vec& rhs) {
        ptr_ = new int[rhs.size_];
        size_ = rhs.size_;
        std::copy(rhs.ptr_, rhs.ptr_ + size_ , ptr_);
    }
    Vec(Vec&& rhs) noexcept {
        ptr_ = std::exchange(rhs.ptr_, nullptr);
        size_ = std::exchange(rhs.size_, 0);
    }
    void swap(Vec& rhs) noexcept {
        std::swap(ptr_, rhs.ptr_);
        std::swap(size_, rhs.size_);
    }

    friend void swap(Vec& lhs, Vec& rhs) noexcept  {
        lhs.swap(rhs);
    }

    Vec& operator=(Vec cpy){
        cpy.swap(*this);
        return *this;
    } 

    ~Vec() {    
        delete [] ptr_;
    }

    friend std::ostream& operator<<(std::ostream &strm, const Vec& v) {
        return strm << "size :" << v.size_ ;
    }

};

class VecU {

    int size ;
    std::unique_ptr<int[]> uptr_ ;
    

public:

    VecU() = default; 
    VecU(const int s) : size(s),  uptr_(std::make_unique<int[]> (s)) {
        for (int i = 0; i< s; i++) 
            uptr_[i] = 2*i;
    }
    VecU(const VecU& rhs): size(rhs.size), uptr_(std::make_unique<int []> (size)) {
        if (size > 0) {
            for (int i = 0; i < size; i++) 
                uptr_[i] = rhs.uptr_[i];
        }
    }
    VecU(VecU&& rhs) = default;

    void swap(VecU& rhs) noexcept {
        std::swap(uptr_, rhs.uptr_);
        std::swap(size, rhs.size);
    }

    friend void swap(VecU& lhs, VecU& rhs) noexcept  {
        lhs.swap(rhs);
    }

    VecU& operator=(VecU cpy) {
        cpy.swap(*this);
        return *this;
    } 
    friend std::ostream& operator<<(std::ostream &strm, const VecU& v) {
        return strm << "size :" << v.size ;
    }
      
    ~VecU() = default;

};

int main() {

    int* arr1 = new int[4]; // {4,3,2,1};
    for(int i = 0; i < 4; i++ )
        arr1[i] = 4-i;
    int* arr2 = new int[6]; //  {6,5,4,3,2,1};
    for(int i = 0; i < 6; i++ )
        arr1[i] = 6-i;
    Vec v1(4, arr1);
    Vec v2(v1);
    Vec v3(std::move(v1));
    Vec v4{};
    v4 = Vec(6, arr2);
    std::cout << v3 << std::endl ;
    std::cout << v4 << std::endl ;


    VecU v5(7);
    VecU v6(std::move(v5));
    VecU v7{} ;
    v7 = v6;
    std::cout << v6 << " , " << v7 << std::endl ;

    return 0;
}