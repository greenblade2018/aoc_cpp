#include "aoc.h"

const std::string FREQS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
using Locations = std::set<Point>;
using FindFunc = void(*)(const StringGrid&, Point, Point, Locations&);

void find_antinodes_one(const StringGrid& grid, Point pos1, Point pos2, Locations& locs) {
    int dr = pos1.first - pos2.first;
    int dc = pos1.second - pos2.second;
    std::vector<Point> nodes {{pos1 + Point(dr, dc)}, {pos2 + Point(-dr, -dc)}};
    if (dr % 3 == 0 && dc % 3 == 0) {
        dr /= 3;
        dc /= 3;
        nodes.emplace_back(pos1.first - dr, pos1.second - dc);
        nodes.emplace_back(pos2.first + dr, pos2.second + dc);
    }
    for (auto n : nodes) {
        if (grid.in_range(n)) locs.insert(n);
    }
}

void find_antinodes_two(const StringGrid& grid, Point pos1, Point pos2, Locations& locs) {
    int dr = pos1.first - pos2.first;
    int dc = pos1.second - pos2.second;
    if (dr == 0) {
        dc = 1;
    } else if (dc == 0) {
        dr = 1;
    } else {
        int g = std::gcd(abs(dr), abs(dc));
        dr /= g;
        dc /= g;
    }

    for (Point p = pos1; grid.in_range(p); p = p + Point(dr, dc)) locs.insert(p);
    for (Point p = pos1; grid.in_range(p); p = p + Point(-dr, -dc)) locs.insert(p);
}

int solve(const StringGrid& grid, FindFunc func) {
    Locations result;
    for (char f : FREQS) {
        auto pos = grid.find_all(f);
        for (size_t i = 0; i < pos.size(); ++i) {
            for (size_t j = i + 1; j < pos.size(); ++j) {
                func(grid, pos[i], pos[j], result);
            }
        }
    }
    std::cout << "antinodes: " << result << std::endl;
    return result.size();
}

int main() {
    assert(FREQS.size() == 26 + 26 + 10);
    StringGrid grid(read_input<std::string>(std::cin, take_string));
    print_answer("one", solve(grid, find_antinodes_one));
    print_answer("two", solve(grid, find_antinodes_two));
}