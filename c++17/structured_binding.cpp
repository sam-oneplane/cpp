// Type your code here, or load an example.
#include <string>
#include <map>
#include <iostream>
#include <utility>

int &add_local_11(std::map<std::string, int> &vars, std::string const &new_var_name)
{
    auto new_vars = vars.insert(std::make_pair(new_var_name, 3));
    if (new_vars.second == false)
    {
        throw std::runtime_error("allready exists.");
    }
    else
    {
        return new_vars.first->second;
    }

}

int &add_local_17(std::map<std::string, int> &vars, std::string const &new_var_name)
{
    /* structured binding */
    if ( auto[itr , success] = vars.insert(std::make_pair(new_var_name, 5)); success == false)
    {
        throw std::runtime_error("allready exists.");
    }
    else
    {
        // auto[key, value] = *itr; 
        // return value;
        return itr->second;
    }   
}

struct Binding
{
    int x1{10};
    int x2{15};
};

int main()
{
    std::map<std::string, int> locals;
    int &x = add_local_11(locals, "sam");
    int &y = add_local_17(locals, "aviv");
    // structured binding with struct members
    auto [i, j] = Binding();
    return x+y+j;
}