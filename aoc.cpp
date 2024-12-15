#include "aoc.h"

//
// Input parsers
//

std::string take_string(std::string& s) {
    return std::move(s);
}

std::vector<int> ints(const std::string& s) {
    std::regex pattern(R"([+-]?\d+)");
    std::vector<int> ret;
    std::transform(std::sregex_iterator(s.cbegin(), s.cend(), pattern),
        std::sregex_iterator(),
        std::back_inserter(ret),
        [](const auto& mr) { return std::stoi(mr.str()); });
    return ret;
}

std::vector<long> longs(const std::string& s) {
    std::regex pattern(R"([+-]?\d+)");
    std::vector<long> ret;
    std::transform(std::sregex_iterator(s.cbegin(), s.cend(), pattern),
        std::sregex_iterator(),
        std::back_inserter(ret),
        [](const auto& mr) { return std::stol(mr.str()); });
    return ret;
}

std::vector<std::string> words(const std::string& s) {
    std::regex pattern(R"([a-zA-z]+)");
    std::vector<std::string> ret;
    std::transform(std::sregex_iterator(s.cbegin(), s.cend(), pattern),
        std::sregex_iterator(),
        std::back_inserter(ret),
        [](const auto& mr) { return mr.str(); });
    return ret;

}

//
// Data structures
//

//
// Positions and directions
//

Point operator+(const Point& a, const Point& b) {
    return Point(a.first + b.first, a.second + b.second);
}

const Point NORTH {0, -1};
const Point SOUTH {0, 1};
const Point EAST {1, 0};
const Point WEST {-1, 0};
const std::map<char, Point> DIRS {{'^', NORTH}, {'v', SOUTH}, {'>', EAST}, {'<', WEST}};
const std::map<char, Point> DIRS_A {{'N', NORTH}, {'S', SOUTH}, {'E', EAST}, {'W', WEST}};

Point make_turn(Point facing, char turn) {
    auto [x, y] = facing;
    if (turn == 'L' || turn == 'l') {
        return Point(y, -x);
    } else if (turn == 'R' || turn == 'r') {
        return Point(-y, x);
    } else {
        // keep current direction
        return facing;
    }
}

const Point RC_NORTH {-1, 0};
const Point RC_SOUTH {1, 0};
const Point RC_EAST {0, 1};
const Point RC_WEST {0, -1};
const std::map<char, Point> RC_DIRS {{'^', RC_NORTH}, {'v', RC_SOUTH}, {'>', RC_EAST}, {'<', RC_WEST}};
const std::map<char, Point> RC_DIRS_A {{'N', RC_NORTH}, {'S', RC_SOUTH}, {'E', RC_EAST}, {'W', RC_WEST}};

Point rc_make_turn(Point facing, char turn) {
    auto [r, c] = facing;
    if (turn == 'L' || turn == 'l') {
        return Point(-c, r);
    } else if (turn == 'R' || turn == 'r') {
        return Point(c, -r);
    } else {
        // keep current direction
        return facing;
    }
}

//
// StringGrid implementation
//

std::ostream& operator<<(std::ostream& os, const StringGrid& grid) {
    os << std::endl;
    for (const auto& line : grid.m_grid) os << line << std::endl;
    return os;
}

const char& StringGrid::operator[](Point pos) const {
    if (!in_range(pos)) {
        throw std::out_of_range(std::format("({}, {}) is out of range.", pos.first, pos.second));
    }
    return m_grid[pos.first][pos.second];
}

// Scott Mayer's const_cast() pattern.
char& StringGrid::operator[](Point pos) {
    return const_cast<char&>(std::as_const(*this)[pos]);
}

std::vector<Point> StringGrid::find_all(char target) const {
    std::vector<Point> result;
    for (size_t r = 0; r < m_grid.size(); ++ r) {
        for (size_t c = 0; c < m_grid[r].size(); ++c) {
            if (m_grid[r][c] == target) result.emplace_back(r, c);
        }
    }
    return result;
}

bool StringGrid::in_range(Point pos) const {
    auto [r, c] = pos;
    return r >= 0 && r < (int)m_grid.size() && c >= 0 && c < (int)m_grid[0].size();
}

std::vector<Point> StringGrid::adjacents(Point pos) const {
    std::vector<Point> result;
    // NOTE: the order is important.
    if (in_range(pos)) {
        auto [r, c] = pos;
        if (r > 0) result.emplace_back(r - 1, c);
        if (c > 0) result.emplace_back(r, c - 1);
        if (c + 1 < (int)m_grid[0].size()) result.emplace_back(r, c + 1);
        if (r + 1 < (int)m_grid.size()) result.emplace_back(r + 1, c);
    }
    return result;
}

std::map<Point, int> StringGrid::bfs(Point src, char empty) const {
    std::map<Point, int> dists;
    if (!in_range(src)) return dists;
    dists[src] = 0;
    std::queue<Point> frontier;
    frontier.push(src);

    while (!frontier.empty()) {
        Point pos = frontier.front();
        frontier.pop();
        if (pos != src && m_grid[pos.first][pos.second] != empty) continue;

        for (Point np : adjacents(pos)) {
            if (!dists.count(np)) {
                dists[np] = dists[pos] + 1;
                frontier.push(np);
            }
        }
    }

    return dists;
}
// End of StringGrid

// Manhattan distance -- usually a good hint function for A* search.
long taxi_distance(const Point& a, const Point& b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}