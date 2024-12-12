#include "aoc.h"

std::set<Point> find_region(const StringGrid& grid, Point pos, StringGrid& visited) {
    assert(visited[pos] == ' ');
    char ch = grid[pos];
    visited[pos] = '*';
    std::deque<Point> que{pos};
    std::set<Point> region{pos};

    while (!que.empty()) {
        auto p = que.front();
        que.pop_front();
        for (auto np : grid.adjacents(p)) {
            if (grid[np] == ch && visited[np] == ' ') {
                visited[np] = '*';
                region.insert(np);
                que.push_back(np);
            }
        }
    }

    return region;
}

std::pair<long, long> find_side(const std::set<Point>& region) {
    long perimeter = 0, side = 0;
    for (Point p: region) {
        auto north = p + RC_NORTH, south = p + RC_SOUTH, west = p + RC_WEST, east = p + RC_EAST;
        auto nw = north + RC_WEST, ne = north + RC_EAST, sw = south + RC_WEST, se = south + RC_EAST;

        if (!region.count(north)) {
            ++perimeter;
            if (!(region.count(west) && !region.count(nw))) ++side;
        }
        if (!region.count(south)) {
            ++perimeter;
            if (!(region.count(west) && !region.count(sw))) ++side;
        }
        if (!region.count(west)) {
            ++perimeter;
            if (!(region.count(north) && !region.count(nw))) ++side;
        }
        if (!region.count(east)) {
            ++perimeter;
            if (!(region.count(north) && !region.count(ne))) ++side;
        }
    }

    return {perimeter, side};
}

int main() {
    StringGrid grid(read_input<std::string>(std::cin, take_string));
    auto [Row, Col] = grid.size();
    std::vector<std::string> tmp(Row, std::string(Col, ' '));
    StringGrid visited(tmp);

    long one = 0, two = 0;
    for (size_t r = 0; r < Row; ++r) {
        for (size_t c = 0; c < Col; ++c) {
            Point p{r, c};
            if (visited[p] != ' ') continue;
            auto region = find_region(grid, p, visited);
            auto [perimeter, side] = find_side(region);
            long price1 = perimeter * region.size(), price2 = side * region.size();
            std::cout << p << ' ' << grid[p] << ' ' << price1 << ' ' << price2 << std::endl;
            one += price1;
            two += price2;
        }
    }

    print_answer("one", one);
    print_answer("two", two);
}