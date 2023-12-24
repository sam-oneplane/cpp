#include <iostream>
#include <unordered_map>
#include <functional>

class UnOrderMap {

public:
    using hashFunc =  std::function<int(const std::string&)> ; 
    
    explicit UnOrderMap(const int alloc, hashFunc func) : myMap(alloc,func) {}
    UnOrderMap() = default;

    void insertValue(const std::pair<std::string, int> keyVal) {
        myMap.insert(std::move(keyVal));
    };
    int findValue(const std::string & val) {
        auto it = myMap.find(val);
        return it->second;
    };
    void deleteValue(const std::string & val) {
        myMap.erase(val);
    }
    void printValues() {

        for(auto it= myMap.begin(); it != myMap.end(); ++it)
            std::cout << "key: " <<it->first << "value: " << it->second << std::endl; 
    }

    void  getBucketCount() {
        std::cout << "bucket count: " << myMap.bucket_count() << std::endl;
    }

    void getKeyValues(const std::string key) {
        for (const auto& element: myMap) {
            if (element.first == key)
                std::cout << element.second << "\t" ;
        }
        std::cout << "\n";
    }

private:
    std::unordered_map<std::string, int, hashFunc> myMap;

};


class UnorderedMultiMap {
public: 
    void printValues() const {
        for(auto it= myMap.begin(); it != myMap.end(); ++it)
            std::cout << "key: " <<it->first << "value: " << it->second << std::endl; 
    }

    void getKeyRange(const int key) const {
        auto range = myMap.equal_range(key);
        for (auto it = range.first; it != range.second; ++it)
            std::cout << "key: " << it->first << ", value: " << it->second << std::endl;
    }

    UnorderedMultiMap(std::unordered_multimap<int, int> && rhs) {
        myMap = rhs;
    }

private:
    std::unordered_multimap<int, int> myMap;

};




int main() {
    UnOrderMap om(8, [](const std::string& s) {
        int sum=0;
        for (char c : s)
            sum+=c;
        return sum%10;
    });
    om.insertValue({"apple", 6});
    om.insertValue({"banana", 8});
    om.insertValue({"pitch", 10});
    om.insertValue({"malon", 13});
    om.insertValue({"apple", 9});
    om.insertValue({"orange", 3});
    om.insertValue({"orange", 2});

    om.deleteValue("malon");
    om.printValues();

    std::cout << "MULTI MAP\n";
    std::unordered_multimap<int, int> myMap {{0, 1}, {0, 10}, {1, 2}, {1, 11}};
    UnorderedMultiMap umm(std::move(myMap));
     
    umm.getKeyRange(0);


 

    return 0;

}