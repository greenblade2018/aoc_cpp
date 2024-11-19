#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "aoc.h"
using namespace aoc;

std::vector<std::string> inputs;

std::map<char, int> histgram(const std::string& s) {
    std::map<char, int> ret;
    for (char c : s) {
        ++ret[c];
    }
    // std::cout << "histgram(" << s << ") => " << ret << std::endl;
    return ret;
}

long solve_one() {
    long count2 = 0, count3 = 0;
    for (const auto& line : inputs) {
        auto h = histgram(line);
        bool has2 = false, has3 = false;
        for (const auto& [k, v] : h) {
            if (v == 2) has2 = true;
            if (v == 3) has3 = true;
        }
        if (has2) ++count2;
        if (has3) ++count3;
    }
    return count2 * count3;
}

int diff(const std::string& s1, const std::string& s2) {
    int ret = 0;
    for (size_t i = 0; i < s1.size(); ++i) {
        if (s1[i] != s2[i]) ++ret;
    }
    return ret;
}

std::string solve_two() {
    size_t first = 0, second = 0;
    for (first = 0; first < inputs.size(); ++first) {
        auto found = false;
        for (second = 0; second < inputs.size(); ++second) {
            if (diff(inputs[first], inputs[second]) == 1) {
                found = true;
                break;
            }
        }
        if (found) break;
    }

    size_t i = 0;
    for (i = 0; i < inputs[first].size(); ++i) {
        if (inputs[first][i] != inputs[second][i]) break;
    }
    return inputs[first].substr(0, i) + inputs[first].substr(i, inputs[first].size() - i);
}

int main() {
    inputs = read_input<std::string>(std::cin, take_string);
    std::cout << "part one: " << solve_one() << std::endl;
    std::cout << "part two: " << solve_two() << std::endl;
}