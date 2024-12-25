#include "aoc.h"
using namespace std;

vector<vector<int>> locks, keys;
int H = 0;

bool is_lock(const vector<string>& lines) {
    assert(lines.size() > 2);
    for (size_t i = 0; i < lines[0].size(); ++i) {
        if (lines[0][i] != '#') return false;
    }
    return true;
}

vector<int> parse_lock(const vector<string>& lines) {
    vector<int> heights;
    for (size_t c = 0; c < lines[0].size(); ++c) {
        size_t r;
        for (r = 1; r < lines.size(); ++r) {
            if (lines[r][c] != '#') break;
        }
        heights.push_back(r - 1);
    }
    return heights;
}

vector<int> parse_key(const vector<string>& lines) {
    vector<int> heights;
    for (size_t c = 0; c < lines[0].size(); ++c) {
        size_t r;
        for (r = lines.size() - 2; r > 0; --r) {
            if (lines[r][c] != '#') break;
        }
        heights.push_back(lines.size() - 2 - r);
    }
    return heights;
}

bool fit(const vector<int>& lock, const vector<int>& key) {
    assert(lock.size() == key.size());
    for (size_t i = 0; i < lock.size(); ++i) {
        if (lock[i] + key[i] > H) return false;
    }
    return true;
}

int main() {
    vector<string> section;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) {
            if (H == 0) H = section.size() - 2;
            if (is_lock(section)) locks.push_back(parse_lock(section));
            else keys.push_back(parse_key(section));
            section.clear();
        } else {
            section.push_back(line);
        }
    }
    if (!section.empty()) {
            if (is_lock(section)) locks.push_back(parse_lock(section));
            else keys.push_back(parse_key(section));
    }
    cout << "locks: " << locks << endl;
    cout << "keys: " << keys << endl;
    cout << format("{} locks, {} keys, H = {}\n", locks.size(), keys.size(), H);

    int one = 0;
    for (const vector<int>& lock : locks) {
        for (const vector<int>& key : keys) {
            if (fit(lock, key)) ++one;
        }
    }
    print_answer("one", one);
}