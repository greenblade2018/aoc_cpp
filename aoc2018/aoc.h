#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <format>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

//
// debug utilities
//

// Need these forward declarations because they call each other.
template <typename K, typename V> std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p);
template <typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);
template <typename T> std::ostream& operator<<(std::ostream& os, const std::set<T>& v);
template <typename K, typename V> std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m);

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << '[';
    bool first = true;
    for (const auto& e : v) {
        if (!first) os << ", ";
        first = false;
        os << e;
    }
    os << ']';

    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& v) {
    os << '{';
    bool first = true;
    for (const auto& e : v) {
        if (!first) os << ", ";
        first = false;
        os << e;
    }
    os << '}';

    return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
    os << '{';
    bool first = true;
    for (const auto& p : m) {
        if (!first) os << ", ";
        first = false;
        os << p;
    }
    os << '}';

    return os;
}

template<typename T>
std::ostream& print_answer(const std::string& part, const T& answer, std::ostream& os = std::cout) {
    os << "part " << part << ": " << answer << std::endl;
    return os;
}

//
// input functions
//

template <typename T>
std::vector<T> read_input(std::istream& is,
                           std::function<T(std::string&)> parse,
                           size_t show = 8) {
    std::vector<T> ret;
    std::string line;
    while (std::getline(is, line)) {
        ret.push_back(parse(line));
    }

    if (show) {
        show = std::min(show, ret.size());
        std::cout << "Parsed input as " << typeid(T).name() << ":\n";
        for (size_t i = 0; i < show; ++i) {
            std::cout << ret[i] << '\n';
        }
        if (show < ret.size()) {
            std::cout << "... first " << show << " of " << ret.size() << " records ...\n";
        } else {
            std::cout << "=== total " << show << " records ===\n";
        }
    }

    return ret;
}

// Take ownership of the line.
std::string take_string(std::string& s);
// Extract all integers with optional signs from string.
std::vector<int> ints(const std::string& s);

//
// Data structures
//

using Point = std::pair<int, int>;
Point operator+(const Point& a, const Point& b);

extern const Point NORTH, EAST, SOUTH, WEST;
extern const std::map<char, Point> DIRS;
extern const std::map<char, Point> DIRS_A;
Point make_turn(Point facing, char turn);

//
// Implement grid as a vector of strings.
// NOTE: it uses (row, column) as coordinate system, not (x, y).
//
class StringGrid final {
    friend std::ostream& operator<<(std::ostream& os, const StringGrid& grid);

public:
    explicit StringGrid(const std::vector<std::string>& lines) : m_grid(lines) {}

    // Will throw out_of_range exception if 'pos' is outside of grid.
    const char& operator[](Point pos) const;
    char& operator[](Point pos);
    // Find all positions for 'target' in the grid.
    std::vector<Point> find_all(char target) const;
    // Check if 'pos' is in the grid range.
    bool in_range(Point pos) const;
    // Find all adjacent (direct up, west, east, down - in that order) positions that are in the grid.
    std::vector<Point> adjacents(Point pos) const;
    // BFS search to find distance to all reachable positions.
    std::map<Point, int> bfs(Point src, char empty = '.') const;

private:
    std::vector<std::string> m_grid;
};
