#include <iostream>
#include <string>
#include <vector>
#include "aoc.h"
using namespace aoc;

std::vector<std::vector<long>> records;

int main() {
    // auto lines = read_lines(std::cin);
    // std::transform(lines.cbegin(),
    //     lines.cend(),
    //     std::back_inserter(records),
    //     ints);
    
    records = read_inputs<std::vector<long>>(std::cin, ints);
    // for (const auto& r : records) std::cout << r << std::endl;
    // std::cout << records << std::endl;
}