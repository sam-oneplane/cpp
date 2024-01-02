// Type your code here, or load an example.
#include <vector>
#include <memory_resource>
#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
    std::byte stack_buf[2048];
    // pmr : polymorfic memory resource
    std::pmr::monotonic_buffer_resource rsrc(stack_buf, sizeof stack_buf);
    // vec is using stack_buf resource
    std::pmr::vector<int> vec {{1,2,3,4,5,6,7}, &rsrc};

    std::ostream_iterator<int> cout_it(std::cout, " ");
    std::copy(vec.rbegin(), vec.rend(), cout_it);

    return 0;
}