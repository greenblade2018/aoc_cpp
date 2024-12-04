#include "aoc.h"

// row, col
const Point DIRS8[] = {
    Point(-1, -1), Point(-1, 0), Point(-1, 1),
    Point(0, -1), Point(0, 1),
    Point(1, -1), Point(1, 0), Point(1, 1),
};

using Grid = std::vector<std::string>;

bool check_dir(const StringGrid& grid, const std::string& s, Point pos, Point dir) {
    for (char c : s) {
        if (!grid.in_range(pos) || grid[pos] != c) return false;
        pos = pos + dir;
    }
    return true;
}

long solve_one(const StringGrid& grid) {
    long result = 0;
    for (const auto& pos : grid.find_all('X')) {
        result += std::count_if(std::cbegin(DIRS8), std::cend(DIRS8),
                                [&grid, &pos](const auto& d) { return check_dir(grid, "XMAS", pos, d); });
    }
    return result;
}

bool check_mas(const StringGrid& grid, Point pos) {
    // X shaped mas, with 'a' in the middle.
    if (grid[pos] != 'A') return false;

    Point ul = pos + DIRS8[0], ur = pos + DIRS8[2], dl = pos + DIRS8[5], dr = pos + DIRS8[7];
    return ((check_dir(grid, "MAS", ul, DIRS8[7]) || check_dir(grid, "MAS", dr, DIRS8[0])) &&
            (check_dir(grid, "MAS", ur, DIRS8[5]) || check_dir(grid, "MAS", dl, DIRS8[2])));
}

long solve_two(const StringGrid& grid) {
    auto as = grid.find_all('A');
    return std::count_if(as.cbegin(), as.cend(), [&grid](const auto& p) { return check_mas(grid, p); });
}

int main() {
    StringGrid grid(read_input<std::string>(std::cin, take_string));
    print_answer("one", solve_one(grid));
    print_answer("two", solve_two(grid));
}