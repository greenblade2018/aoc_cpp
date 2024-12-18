#include "aoc.h"
using namespace std;

Point parse_point(const string& s) {
    auto v = ints(s);
    return Point{v[0], v[1]};
}

bool in_memory(Point p, int size) {
    return 0 <= p.first && p.first <= size && 0 <= p.second && p.second <= size;
}

int one(const vector<Point>& points, int fall, int size) {
    set<Point> marks;
    for (int i = 0; i < fall; ++i) marks.insert(points[i]);
    Point src{0, 0}, dst{size, size};
    if (marks.count(src) || marks.count(dst)) return -1;

    map<Point, int> dists;
    dists[src] = 0;
    queue<Point> frontier;
    frontier.push(src);

    while (!frontier.empty()) {
        auto pos = frontier.front();
        frontier.pop();

        for (auto [_, dir] : DIRS) {
            Point np = pos + dir;
            if (in_memory(np, size) && !marks.count(np) && !dists.count(np)) {
                dists[np] = dists[pos] + 1;
                frontier.push(np);
            }
        }
    }

    return dists.count(dst) ? dists.at(dst) : -1;
}

Point two(const vector<Point>& points, int size) {
    int low = 0, high = points.size();
    assert(one(points, high, size) < 0);

    while (low + 1 < high) {
        int mid = (low + high) / 2;
        if (one(points, mid, size) < 0) high = mid;
        else low = mid;
    }
    return points[high-1];
}

int main() {
    auto points = read_input<Point>(cin, parse_point);
    // print_answer("one", one(points, 12, 6));
    // print_answer("two", two(points, 6));
    print_answer("one", one(points, 1024, 70));
    print_answer("two", two(points, 70));
}