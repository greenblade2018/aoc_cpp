#include "aoc.h"

using Mul = std::pair<int, int>;

std::vector<Mul> parse_line(const std::string& line) {
    static std::regex pat(R"(mul\((\d+),(\d+)\))");
    std::vector<Mul> result;
    std::transform(std::sregex_iterator(line.cbegin(), line.cend(), pat),
        std::sregex_iterator(),
        std::back_inserter(result),
        [](const auto& mr) { return Mul(std::stoi(mr.str(1)), std::stoi(mr.str(2))); });
    return result;
}

std::vector<Mul> parse_two(const std::string& line) {
    static std::regex pat(R"(mul\((\d+),(\d+)\)|do(n't)?\(\))");
    std::vector<Mul> result;
    std::sregex_iterator end;
    static bool enable = true;
    for (std::sregex_iterator it(line.cbegin(), line.cend(), pat); it != end; ++it) {
        // std::cout << it->str(0) << std::endl;   
        auto prefix = it->str(0).substr(0, 3);
        std::cout << prefix << std::endl;
        if (prefix == "mul") {
            if (enable) result.emplace_back(std::stoi(it->str(1)), std::stoi(it->str(2)));
        }
        else if (prefix == "don") {
            enable = false;
        } else {
            enable = true;
        }
    }
    return result;
}

long solve_one(const std::vector<std::vector<Mul>>& records) {
    long result = 0;
    for (const auto& muls : records) {
        for (auto [a, b] : muls) result += (long)a * b;
    }
    return result;
}

int main() {
    std::string line = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
    auto records = read_input<std::vector<Mul>>(std::cin, parse_two);
    print_answer("one", solve_one(records));
 }