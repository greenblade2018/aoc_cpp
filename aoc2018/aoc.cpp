#include "aoc.h"

namespace aoc {

//
// Input parsers
//

std::string take_string(std::string& s) {
    return std::move(s);
}

std::vector<long> ints(const std::string& s) {
    std::regex pattern(R"([+-]?\d+)");
    std::vector<long> ret;
    std::transform(std::sregex_iterator(s.cbegin(), s.cend(), pattern),
        std::sregex_iterator(),
        std::back_inserter(ret),
        [](const auto& mr) { return std::stol(mr.str()); });
    return ret;
}

//
// Data structures
//

Point operator+(const Point& a, const Point& b) {
    return Point(a.first + b.first, a.second + b.second);
}

const Point NORTH {0, -1};
const Point SOUTH {0, 1};
const Point EAST {1, 0};
const Point WEST {-1, 0};
const std::map<char, Point> DIRS {{'^', NORTH}, {'v', SOUTH}, {'>', EAST}, {'<', WEST}};

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

} // namespace aoc