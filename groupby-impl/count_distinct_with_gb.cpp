#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <stdexcept>

constexpr int ONE_LINE = 1;
constexpr int EMPTY_TABLE = 0;

template<typename T>
struct Table {
    int cols;
    typedef std::vector<T> Row;
    typedef std::map<int, std::set<T>> Groups;
    

    std::vector<Row> table;
    Table(int c): cols(c) {};

    std::function<void(std::set<T>&, int)> dist_values = [](std::set<T>& group, int col) {
        
        std::cout  << "column: " << col << "  distinct values: " << group.size() <<"\n"; 
    };
    

    bool compareby_columns(Row table_row1, Row table_row2, std::list<int>& cols_l) {
        std::list<int>::iterator it;
        Row row1, row2;
        for (it=cols_l.begin(); it != cols_l.end() ; it++) {
            row1.push_back(table_row1[*it]);
        }
        for (auto it=cols_l.begin(); it != cols_l.end() ; it++) {
            row2.push_back(table_row2[*it]);
        }
        return (row1 == row2);
    }


    int groupby_func(std::list<int> cols_l, const int col) {
        typename std::vector<Row>::iterator row_it ;
        
        // INIT
        row_it = table.begin();
        // insert first element to first group
        std::set<T> group;
        group.insert((*row_it++)[col]);

        if (table.size() == ONE_LINE) {
            // insert single row and return 
            dist_values(group, col);
            return 0;
        }
        for (; row_it < table.end(); row_it++) {
            if (compareby_columns(*row_it, *(row_it-1), cols_l)) {
                // upatde group with the same key 
                group.insert((*row_it)[col]);
            }else {
                // insert group to map and clear group 
                dist_values(group, col);
                group.clear();
                // insert new element to group
                group.insert((*row_it)[col]);
            }
        }    
        // insert the last group 
        dist_values(group, col);
        return 0;
    }



    void countDistinct(std::list<int> groupby_list, int col) {

        try {        
            if(table.size() == EMPTY_TABLE) {
                std::string msg = "Table id empty";
                throw msg ;
            }
            Row row = table.front();
            if ((col > static_cast<int>(row.size()) ) || (col < 0) ) {
                std::string msg =  "column value is out of scope";
                throw msg;
            }
            groupby_func(groupby_list, col);
        }catch (std::string& msg) {
            std::cerr << msg << std::endl;
        }        
    }


    void insertRow(std::vector<T> row) {
        try {
            if (static_cast<int>(row.size()) != cols) { 
                std::string msg = "wrong num of colums!!";
                throw msg;
            }
            Row row_t(row);

            table.push_back(std::move(row_t));

        }catch(std::string& msg) {
            std::cerr << msg << std::endl;
        }catch(std::runtime_error& err) {
            std::cerr << err.what() << std::endl;
        }catch(std::bad_alloc& err) {
            std::cerr << err.what() << std::endl;
        }        
    } 
};


void test1() {
    Table<int> table(2);
    table.insertRow({0,7});
    table.insertRow({0,8});
    table.insertRow({1,8});
    table.insertRow({2,9,11});
    table.insertRow({2,9});
    table.insertRow({});

    std::list<int> group_by = {0};
    std::cout << "Table dim : " << table.table.size() << " x " << table.cols << std::endl;

    table.countDistinct(group_by, 1); // distict values for each group on column 1
}

void test2() {
    
    Table<int> table(3);
    table.insertRow({0,2,6});
    table.insertRow({0,2,8});
    table.insertRow({1,3,9});
    table.insertRow({1,4,9});

    std::list<int> group_by = {0,2};
    std::cout << "Table dim : " << table.table.size() << " x " << table.cols << std::endl;

    table.countDistinct(group_by, 1); // distict values for each group on column 1
    
}

void test3() {

    Table<float> table(2);
    table.insertRow({0.5,7.2});
    table.insertRow({0.5,8.1});
    table.insertRow({1.1,8.1});
    table.insertRow({2.6,9.3});

    std::list<int> group_by = {0};
    std::cout << "Table dim : " << table.table.size() << " x " << table.cols << std::endl;

    table.countDistinct(group_by, 1); // distict values for each group on column 1
}

void test4() {

    Table<std::string> table(2);
    table.insertRow({"aa","bb"});
    table.insertRow({"aa","bc"});
    table.insertRow({"ba","bc"});

    std::list<int> group_by = {0};
    std::cout << "Table dim : " << table.table.size() << " x " << table.cols << std::endl;

    table.countDistinct(group_by, 1); // distict values for each group on column 1
}


void test5() {
    
    Table<int> table(3);
    table.insertRow({0,2,6});

    std::list<int> group_by = {0,2};
    std::cout << "Table dim : " << table.table.size() << " x " << table.cols << std::endl;

    table.countDistinct(group_by, 1); // distict values for each group on column 1
    
}


void test6() {
    
    Table<int> table(3);
    table.insertRow({0,2,6});
    table.insertRow({0,2,8});
    table.insertRow({1,3,9});
    table.insertRow({1,4,9});

    std::list<int> group_by = {0,2};
    std::cout << "Table dim : " << table.table.size() << " x " << table.cols << std::endl;

    table.countDistinct(group_by, 4); // distict values for each group on column 1
    
}

void test7() {

    Table<int> table(2);

    std::list<int> group_by = {0};
    std::cout << "Table dim : " << table.table.size() << " x " << table.cols << std::endl;

    table.countDistinct(group_by, 1); // distict values for each group on column 1
}

int main() {

    
    test1();
    std::cout << "\n\n" ;
    test2();
    std::cout << "\n\n" ;
    test3();
    std::cout << "\n\n" ;
    test4();
    std::cout << "\n\n" ;
    test5();
    std::cout << "\n\n" ;
    test6();
    std::cout << "\n\n" ;
    test7();
    return 0;
}

