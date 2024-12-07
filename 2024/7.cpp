#include "aoc.h"

using Equation = std::vector<long>;
using Equations = std::vector<std::vector<long>>;

bool concat = false;

std::set<long> generate(Equation nums, size_t n) {
    if (n == 1) return std::set<long>{nums[1]};

    std::set<long> results;
    for (auto x : generate(nums, n - 1)) {
        results.insert(x + nums[n]);
        results.insert(x * nums[n]);
        if (concat) {
            auto s = std::to_string(x) + std::to_string(nums[n]);
            long y = std::stol(s);
            if (y <= nums[0]) results.insert(y);
        }
    }
    return results;
}

long one(const Equations& equations) {
    long result = 0;
    for (const auto& eq: equations) {
        auto r = generate(eq, eq.size() - 1);
        if (r.count(eq[0])) result += eq[0];
    }
    return result;
}

int main() {
    auto equations = read_input<Equation>(std::cin, longs);
    print_answer("one", one(equations));
    concat = true;
    print_answer("two", one(equations));
}