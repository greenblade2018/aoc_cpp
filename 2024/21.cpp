#include "aoc.h"
using namespace std;

StringGrid num_pad(vector<string>{"789", "456", "123", "*0A"});
StringGrid move_pad(vector<string>{"*^A", "<v>"});

char find_dir(Point from, Point to) {
    for (auto [c, d] : RC_DIRS) {
        if (from + d == to) return c;
    }
    assert(false);
    return '!';
}

void build_paths(Point dst, const map<Point, set<Point>>& prevs, string& current, vector<string>& paths) {
    // cout << dst << ' ' << prevs.at(dst) << endl;
    if (prevs.at(dst).empty()) {
        // cout << "adding " << current << endl;
        string tmp(current.rbegin(), current.rend());
        tmp.push_back('A');
        paths.emplace_back(tmp);
        return;
    }

    for (auto p : prevs.at(dst)) {
        current.push_back(find_dir(p, dst));
        build_paths(p, prevs, current, paths);
        current.pop_back();
    }
    return;
}

vector<string> all_short(const StringGrid& grid, char from, char to) {
    auto src = grid.find_all(from).at(0);
    auto dst = grid.find_all(to).at(0);

    map<Point, int> dists;
    dists[src] = 0;
    map<Point, set<Point>> prevs;
    prevs[src] = set<Point>();
    queue<Point> frontier;
    frontier.push(src);

    while (!frontier.empty()) {
        auto pos = frontier.front();
        frontier.pop();
        if (pos == dst) break;

        for (auto np : grid.adjacents(pos)) {
            if (grid[np] == '*') continue;
            int nd = dists[pos] + 1;
            if (!dists.count(np)) {
                dists[np] = nd;
                assert(!prevs.count(np));
                prevs[np].insert(pos);
                frontier.push(np);
            } else if (dists[np] == nd) {
                prevs[np].insert(pos);
            } else {
                // do nothing
            }
        }
    }

    assert(dists.count(dst) && prevs.count(dst));
    vector<string> paths;
    string tmp;
    build_paths(dst, prevs, tmp, paths);
    return paths;
}

const string num_str = "1234567890A";
const string move_str = "^v<>A";
map<pair<char, char>, vector<string>> num_dict, move_dict;

map<pair<string, int>, long long> cache;

long long do_move(string path, int n) {
    if (n == 25) return path.size();
    auto key = make_pair(path, n);
    if (cache.count(key)) return cache[key];

    char prev = 'A';
    long long result = 0;
    for (char c : path) {
        long long tmp = 2'000'000'000'000'000'000;
        for (const auto& new_path : move_dict[{prev, c}]) {
            tmp = min(tmp, do_move(new_path, n + 1));
        }
        prev = c;
        result += tmp;
    }
    cache[key] = result;
    return result;
}

long long do_num(string key) {
    char prev = 'A';
    long long result = 0;
    for (char c : key) {
        long long tmp = 2'000'000'000'000'000'000;
        for (const auto& path : num_dict[{prev, c}]) {
            tmp = min(tmp, do_move(path, 0));
        }
        prev = c;
        result += tmp;
    }
    return result;
}

int main() {
    // string s = "A029A7";
    // for (size_t i = 1; i < s.size(); ++i) {
    //     auto paths = all_short(num_pad, s[i-1], s[i]);
    //     cout << s[i-1] << " => " << s[i] << ": " << paths << endl;
    // }

    for (char a : num_str) {
        for (char b : num_str) num_dict[{a, b}] = all_short(num_pad, a, b);
    }
    for (char a : move_str) {
        for (char b : move_str) move_dict[{a, b}] = all_short(move_pad, a, b);
    }
    cout << format("num_dict = {}, move_dict = {}\n", num_dict.size(), move_dict.size());

    auto lines = read_input<string>(cin, take_string);
    long long one = 0;
    for (const auto& s : lines) {
        long long n = stoll(s);
        long long x = do_num(s);
        cout << format("{} => {} {} {}\n", s, x, n, n * x);
        one += n * x;
    }
    print_answer("one", one);
}