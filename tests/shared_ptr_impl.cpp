#include <iostream>

template<typename T> 
class SharedPtr {
    using iter = T*;
    using const_iter = const T*;
    iter ptr = nullptr;
    int *rc = 0;

public: 
    explicit SharedPtr(iter p): ptr{p}, rc{new int(1)} {}
    // use copy constructor for initialization
    SharedPtr(const SharedPtr<T>& other) : ptr(other.ptr), rc(other.rc) {
        (*rc)++;
    }
    SharedPtr(SharedPtr<T>&& other) {
        // replace the value of rhs with nullptr and return the old value into ptr 
        ptr = std::exchange(rhs.ptr, nullptr);
        rc = std::exchange(rhs.rc, nullptr);
    }

    void swap(SharedPtr<T>& other) {    
        std::swap(ptr, other.ptr);
        std::swap(rc, other.rc);
    }
    
    friend void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept  {
        lhs.swap(rhs);
    }

    SharedPtr& operator=(SharedPtr<T> copy)  {
        copy.swap(*this);
        this->rc++;
        return *this
    }
    
    
    int getRc() {
        return *rc;
    }

    // RAII
    ~SharedPtr() { 
        /* dec reference counter by 1*/
        (*rc)--;
        std::cout << "dec rc\n";
        if(*rc == 0) {
            delete rc;
            delete ptr; 
            std::cout << "rc is zero, delete alloc mem in heap\n";
        }else{
            ptr=nullptr;
        }
    }
};


int main() {

    /* create a new shared ptr for int with value 10*/
    SharedPtr<int> sp0(new int(10));
    {
        SharedPtr<int> sp1 = sp0;
        std::cout << "rc for sp1 = " << sp1.getRc() << "\n";
    }
    std::cout << "rc for sp0 = " << sp0.getRc() << "\n";

    SharedPtr<int> sp2= sp0;
    std::cout << "rc for sp2 = " << sp2.getRc() << "\n";
    return 0 ;
}

