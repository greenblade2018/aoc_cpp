#include "aoc.h"

using Report = std::vector<int>;

bool adjacent_diff(const Report& v, int low, int high, size_t skip = 32768) {
    for (size_t i = 1; i < v.size(); ++i) {
        if (i == skip) continue;
        int j = i - 1;
        if (j == (int)skip) --j;
        if (j < 0) continue;

        if (int d = v[i] - v[j]; d < low || d > high) return false;
    }
    return true;
}

bool safe(const Report& r) {
    return adjacent_diff(r, 1, 3) || adjacent_diff(r, -3, -1);
}

bool tolerate(const Report& r) {
    if (r.size() <= 1 || safe(r)) return true;
    for (size_t i = 0; i < r.size(); ++i) {
        if (adjacent_diff(r, 1, 3, i) || adjacent_diff(r, -3, -1, i)) return true;
    }
    return false;
}

int solve_one(const std::vector<Report>& reports) {
    return std::count_if(reports.begin(), reports.end(), safe);
}

int solve_two(const std::vector<Report>& reports) {
    return std::count_if(reports.begin(), reports.end(), tolerate);
}


int main() {
    auto reports = read_input<std::vector<int>>(std::cin, ints);
    print_answer("one", solve_one(reports));
    print_answer("two", solve_two(reports));
}