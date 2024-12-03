#include <format>
#include <iostream>
#include <string>

#include "aoc.h"


bool trigger(char a, char b) {
    if (a < b) std::swap(a, b);
    bool ret = (a - b) == ('a' - 'A');
    // std::cerr << std::format("trigger({}, {}) => {}", a, b, ret) << std::endl;
    return ret;
}

int react(const std::string& src, const std::string& ignore = "**") {
    std::string ret;

    for (size_t i = 0; i < src.size(); ++i) {
        if (src[i] == ignore[0] || src[i] == ignore[1]) continue;

        if (!ret.empty() && trigger(ret.back(), src[i])) {
            ret.pop_back();
        } else {
            ret.push_back(src[i]);
        }
    }

    return ret.size();
}

int main() {
    std::string line;
    std::getline(std::cin, line);
    std::cout << "part one: " << react(line) << std::endl;

    std::string ignore = "aA";
    int two = 1'000'000'000;
    while (ignore[0] <= 'z') {
        two = std::min(two, react(line, ignore));
        ++ignore[0];
        ++ignore[1];
    }
    std::cout << "part two: " << two << std::endl;
}