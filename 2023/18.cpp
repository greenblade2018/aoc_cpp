#include "aoc.h"
using namespace std;

//
// Impose a coordinate system on the grid with grid points at the center of the square ('#').
// 
// Pick's theorem: Suppose you have a polygon with integer coordinates with area A, and
// B integer points along its boundary, and I integer points strictly within.
// Then A = I + B/2 - 1. This means I = A - B/2 + 1.
// In this problem, we need I + B, which is A + B/2 + 1.
//
// To calculate area, use shoelace formula: A = (\sum X[i]*Y[i+1] - X[i+1]*Y[i]) / 2.
// If the points are given clock-wise, then the area is abs(A).
//

struct Record {
    char dir;
    int len;
    string color;
};

Record parse_record(const string& s) {
    auto space = s.rfind(' ');
    Record r;
    r.dir = s[0];
    r.len = stoi(s.substr(2, space - 2));
    r.color = s.substr(space + 3, s.size() - space - 4);
    return r;
}

ostream& operator<<(ostream& os, const Record& r) {
    return os << format("Record{{{}, {}, {}}}", r.dir, r.len, r.color);
}

const map<char, Point> MYDIRS {{'U', NORTH}, {'D', SOUTH}, {'L', WEST}, {'R', EAST}};

long calc_area(const vector<Point>& points) {
    assert(points.front() == points.back());
    int n = points.size() - 1;
    long area = 0;
    for (int i = 0; i < n; ++i) {
        area += (long)points[i].first * points[i+1].second - (long)points[i+1].first * points[i].second;
    }
    if (area < 0) area = -area;
    assert(area % 2 == 0);
    return area / 2;
}

void parse_color(const string& s, char& dir, int& len) {
    int idx = s.back() - '0';
    assert(0 <= idx && idx < 4);
    dir = "RDLU"[idx];
    len = stol(s.substr(0, s.size() - 1), nullptr, 16);
}

long solve(const vector<Record>& records, int part) {
    Point zero{0, 0};
    vector<Point> points{zero};
    long perimeter = 0;
    for (const auto& r : records) {
        char dir = ' ';
        int len = 0;
        if (part == 1) {
            dir = r.dir;
            len = r.len;
        } else {
            parse_color(r.color, dir, len);
        }

        perimeter += len;
        auto d = MYDIRS.at(dir);
        d.first *= len;
        d.second *= len;
        points.push_back(points.back() + d);
    }
    assert(points.back() == zero);

    auto area = calc_area(points);
    return area + perimeter / 2 + 1;
}

int main() {
    auto records = read_input<Record>(cin, parse_record);
    print_answer("one", solve(records, 1));
    print_answer("two", solve(records, 2));
}