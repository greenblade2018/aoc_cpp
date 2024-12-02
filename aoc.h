#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <format>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
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

//
// Search algorithms and data structures
//

//
// A node in the search queue.
//
template<typename S>
struct SearchNode {
    S state;
    long cost = 0;  // Cost to reach current state.
    long total_cost = 0;  // Estimated total cost when reaching goal.

    static const long FAILURE = std::numeric_limits<long>::max();
    auto operator<=>(const SearchNode<S>& other) const { return total_cost <=> other.total_cost; }
};

template<typename S>
std::ostream& operator<<(std::ostream& os, const SearchNode<S>& node) {
    return os << "SearchNode(" << node.state << ", " << node.cost << ")";
}

//
// Abstract base class for search problems.
//

template <typename S>
class SearchProblem {
public:
    using Action = std::pair<S, long>;  // new_state, cost

    virtual S initial_state() = 0;
    virtual bool is_goal(const S& state) = 0;
    virtual std::vector<Action> actions(const S& state) = 0;
    // hint function for A* search.
    // Must alway be <= real cost from this state to goal.
    virtual long hfunc(const S& state) { return 0; }    // zero means dijkstra search
    virtual ~SearchProblem() = default;
};

//
// A* search algorithm.
//

template<typename S>
SearchNode<S> a_star_search(SearchProblem<S>& problem) {
    SearchNode<S> node = {problem.initial_state(), 0};
    std::priority_queue<SearchNode<S>, std::vector<SearchNode<S>>, std::greater<SearchNode<S>>> frontier;
    frontier.push(node);
    std::map<S, long> visited = {{node.state, 0}};

    size_t expanded = 0;
    while (!frontier.empty()) {
        node = frontier.top();  // reuse the node object
        frontier.pop();
        // std::cout << node << std::endl;
        if (problem.is_goal(node.state)) {
            std::cout << "[SUCCESS] Nodes expanded: " << expanded << std::endl;
            return node;
        }

        ++expanded;
        for (const auto& [new_state, cost] : problem.actions(node.state)) {
            auto new_cost = node.cost + cost;
            if (!visited.count(new_state) || visited[new_state] > new_cost) {
                visited[new_state] = new_cost;
                frontier.emplace(new_state, new_cost, new_cost + problem.hfunc(new_state));
            }
        }
    }

    std::cout << "[FAIL] Nodes expanded: " << expanded << std::endl;
    return SearchNode<S>{S(), SearchNode<S>::FAILURE};
}

// Manhattan distance -- usually a good hint function for A* search.
long taxi_distance(const Point& a, const Point& b);
