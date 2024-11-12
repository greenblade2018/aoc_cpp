#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "aoc.h"

namespace aoc {

//
// input parsers
//

std::string take_string(std::string& s) {
    return std::move(s);
}

std::vector<long> ints(const std::string& s) {
    std::regex pattern(R"([+-]?\d+)");
    std::vector<long> ret;
    std::transform(std::sregex_iterator(s.cbegin(), s.cend(), pattern),
        std::sregex_iterator(),
        std::back_inserter(ret),
        [](const auto& mr) { return std::stol(mr.str()); });
    return ret;
}

} // namespace aoc