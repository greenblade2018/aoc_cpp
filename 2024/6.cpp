#include "aoc.h"

using Track = std::set<std::pair<Point, Point>>;

bool get_track(const StringGrid& grid, Point pos, Point dir, Track& track) {
    while (true) {
        // std::cout << "pos = " << pos << ", dir = " << dir << std::endl;
        auto tmp = std::make_pair(pos, dir);
        if (track.count(tmp)) return true;
        track.insert(tmp);

        auto new_pos = pos + dir;
        if (!grid.in_range(new_pos)) return false;
        if (grid[new_pos] == '#') {
            dir = rc_make_turn(dir, 'R');
        } else {
            pos = new_pos;
        }
    }
    // unreachable
}

int main() {
    StringGrid grid(read_input<std::string>(std::cin, take_string));

    Track track;
    auto start = grid.find_all('^')[0];
    assert(get_track(grid, start, RC_NORTH, track) == false);
    std::set<Point> positions;
    for (const auto& p : track) positions.insert(p.first);
    print_answer("one", positions.size());

    int two = 0;
    for (auto p : positions) {
        if (p == start) continue;
        track.clear();
        grid[p] = '#';
        if (get_track(grid, start, RC_NORTH, track)) ++two;
        grid[p] = '.';
    }
    print_answer("two", two);
}