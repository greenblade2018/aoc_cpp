#include <format>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::ostream_iterator<int> out_it(std::cout, ", "); // Create an ostream_iterator for cout with a space delimiter

    // Use the ostream_iterator to output the vector elements
    std::copy(numbers.begin(), numbers.end(), out_it); 
    std::cout << std::endl;

    std::vector<int> v{6};
    std::copy(v.begin(), v.end(), out_it); 
    std::cout << std::endl;

    std::cout << std::format("{}", v) << std::endl;;
    return 0;
}