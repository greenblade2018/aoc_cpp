#include <iostream>
#include <string>
#include <vector>
#include "aoc.h"
using namespace aoc;

using Claim = std::vector<long>;
std::vector<Claim> inputs;
using Grid = std::map<Point, int>;
Grid grid;

void run_claim(const Claim& claim) {
    long x0 = claim[1], y0 = claim[2], W = claim[3], H = claim[4];
    for (long x = x0; x < x0 + W; ++x) {
        for (long y = y0; y < y0 + H; ++y) {
            ++grid[Point(x, y)];
        }
    }
}

long solve_one() {
    for (const auto& claim : inputs) run_claim(claim);
    long ret = 0;
    for (const auto& [_, v] : grid) {
        if (v > 1) ++ret;
    }
    return ret;
}

bool check_claim(const Claim& claim) {
    long x0 = claim[1], y0 = claim[2], W = claim[3], H = claim[4];
    for (long x = x0; x < x0 + W; ++x) {
        for (long y = y0; y < y0 + H; ++y) {
            if (grid[Point(x, y)] > 1) return false;
        }
    }
    return true;
}

int solve_two() {
    for (const auto& claim : inputs) {
        if (check_claim(claim)) return claim[0];
    }
    return -1;
}

int main() {
    inputs = read_inputs<Claim>(std::cin, ints);
    std::cout << "part one: " << solve_one() << std::endl;
    std::cout << "part two: " << solve_two() << std::endl;
}