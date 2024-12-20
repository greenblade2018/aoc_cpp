#include "aoc.h"
using namespace std;

map<Point, int> bfs(const StringGrid& grid, Point src) {
    map<Point, int> dists;
    dists[src] = 0;
    queue<Point> frontier;
    frontier.push(src);

    while (!frontier.empty()) {
        auto pos = frontier.front();
        frontier.pop();
        for (Point np : grid.adjacents(pos)) {
            if (grid[np] == '#' || dists.count(np)) continue;
            dists[np] = dists[pos] + 1;
            frontier.push(np);
        }
    }
    return dists;
}

void cheat(const StringGrid& grid, const map<Point, int>& dists, Point src, map<pair<Point, Point>, int>& results, int length) {
    map<Point, int> mydists;
    mydists[src] = 0;
    queue<Point> frontier;
    frontier.push(src);

    while (!frontier.empty()) {
        Point pos = frontier.front();
        frontier.pop();
        if (mydists[pos] >= length) break;

        for (Point np : grid.adjacents(pos)) {
            if (!mydists.count(np)) {
                mydists[np] = mydists[pos] + 1;
                frontier.push(np);
            }
        }
    }

    for (auto [p, d] : mydists) {
        assert(d <= 20);
        if (grid[p] == '#') continue;
        auto k = make_pair(src, p);
        int save = dists.at(p) - dists.at(src) - mydists[p];
        if (save > 0) results[k] = save;
    }
}

int main() {
    auto lines = read_input<string>(cin, take_string);
    StringGrid grid(lines);

    Point start = grid.find_all('S')[0];
    Point end = grid.find_all('E')[0];
    cout << "start: " << start << ", end: " << end << endl;
    auto dists = bfs(grid, start);
    // cout << "dists: " << dists << endl;

    map<pair<Point, Point>, int> results;
    for (const auto& p : dists) {
        cheat(grid, dists, p.first, results, 20);
    }
    // cout << "results: " << results << endl;

    int one = 0;
    map<int, int> stats;
    for (const auto& p : results) {
        if (p.second > 0) ++stats[p.second];
        if (p.second >= 100) ++one;
    }
    for (auto [k, v] : stats) {
        cout << format("{} save {} picoseconds.\n", v, k);
    }
    print_answer("one", one);
}