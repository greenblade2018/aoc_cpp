#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace aoc {

//
// debug utilities
//

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << '[';
    bool first = true;
    for (const auto& e : v) {
        if (!first) os << ", ";
        first = false;
        os << e;
    }
    os << ']';

    return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p) {
    os << '(' << p.first << ", " << p.second << ')';
    return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
    os << '{';
    bool first = true;
    for (const auto& p : m) {
        if (!first) os << ", ";
        first = false;
        os << p;
    }
    os << '}';

    return os;
}

//
// input functions
//

template <typename T>
std::vector<T> read_inputs(std::istream& is,
                           std::function<T(std::string&)> parse,
                           size_t show = 8) {
    std::vector<T> ret;
    std::string line;
    while (std::getline(is, line)) {
        ret.push_back(parse(line));
    }

    if (show) {
        show = std::min(show, ret.size());
        std::cout << "Parsed input as:\n";
        for (size_t i = 0; i < show; ++i) {
            std::cout << ret[i] << '\n';
        }
        if (show < ret.size()) {
            std::cout << "... first " << show << " records ...\n";
        } else {
            std::cout << "=== total " << show << " records ===\n";
        }
    }

    return ret;
}

// Take ownership of the line.
std::string take_string(std::string& s);
// Extract all integers with optional signs from string.
std::vector<long> ints(const std::string& s);


//
// Data structures
//

using Point = std::pair<int, int>;

} // namespace aoc