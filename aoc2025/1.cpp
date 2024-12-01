#include "aoc.h"

namespace ranges = std::ranges;
namespace rv = std::ranges::views;

long solve_one(const std::vector<int>& left, const std::vector<int>& right) {
    long result = 0;
    for (size_t i = 0; i < left.size(); ++i) {
        result += abs(left[i] - right[i]);
    }
    return result;
}

long solve_two(const std::vector<int>& left, const std::vector<int>& right) {
    std::map<int, int> histgram;
    for (auto n : right) ++histgram[n];

    long score = 0;
    for (auto n : left) score += n * histgram[n];
    return score;
}

int main() {
    std::vector<int> left, right;
    std::string line;
    while (getline(std::cin, line)) {
        auto v = ints(line);
        left.push_back(v[0]);
        right.push_back(v[1]);
    }

    ranges::sort(left);
    ranges::sort(right);
    std::cout << "left  = " << left << std::endl;
    std::cout << "right = " << right << std::endl;

    print_answer("one", solve_one(left, right));
    print_answer("two", solve_two(left, right));
}