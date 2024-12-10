#include "aoc.h"

int bfs(const StringGrid& grid, Point start) {
    std::set<Point> visited{start};
    std::deque<Point> que{start};
    while (!que.empty()) {
        auto pos = que.front();
        que.pop_front();

        char c = grid[pos] + 1;
        for (Point new_pos : grid.adjacents(pos)) {
            if (grid[new_pos] == c && !visited.count(new_pos)) {
                visited.insert(new_pos);
                que.push_back(new_pos);
            }
        }
    }

    return std::count_if(visited.cbegin(), visited.cend(),
        [&grid](const auto& p) { return grid[p] == '9'; });
}

int bfs2(const StringGrid& grid, Point start) {
    std::map<Point, int> ratings{{start, 1}};
    std::deque<Point> que{start};
    while (!que.empty()) {
        auto pos = que.front();
        que.pop_front();

        char c = grid[pos] + 1;
        assert(ratings.count(pos));
        int r = ratings[pos];
        for (Point new_pos : grid.adjacents(pos)) {
            if (grid[new_pos] == c) {
                if (!ratings.count(new_pos)) que.push_back(new_pos);
                ratings[new_pos] += r;
            }
        }
    }

    int score = 0;
    for (const auto& [p, r] : ratings) {
        if (grid[p] == '9') score += r;
    }
    return score;
}

using BFunc = int(*)(const StringGrid&, Point);

int solve(const StringGrid& grid, BFunc func) {
    int result = 0;
    for (Point p : grid.find_all('0')) result += func(grid, p);
    return result;
}

int main() {
    StringGrid grid(read_input<std::string>(std::cin, take_string));
    print_answer("one", solve(grid, bfs));
    print_answer("two", solve(grid, bfs2));
}