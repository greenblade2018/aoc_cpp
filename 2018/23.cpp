#include "aoc.h"

using Nanobot = std::vector<int>;

long distance(const Nanobot& a, const Nanobot& b) {
    return std::abs(a[0] - b[0]) + std::abs(a[1] - b[1]) + std::abs(a[2] - b[2]);
}

int solve_one(const std::vector<Nanobot>& bots) {
    Nanobot best = *std::max_element(bots.cbegin(), bots.cend(),
        [](const Nanobot& a, const Nanobot& b) { return a[3] < b[3]; });
    std::cout << best << std::endl;
    return std::count_if(bots.cbegin(), bots.cend(),
        [&best](const Nanobot& a) { return distance(a, best) <= best[3]; });
}

int main() {
    auto bots = read_input<Nanobot>(std::cin, ints);
    print_answer("one", solve_one(bots));
}