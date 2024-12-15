#include "aoc.h"
using namespace std;

void parse_input(vector<string>& lines, string& moves) {
    string line;
    while (true) {
        getline(cin, line);
        if (line.empty()) break;
        lines.push_back(line);
    }

    while (getline(cin, line)) moves += line;
}

long total_gps(const StringGrid& grid, char ch) {
    long result = 0;
    for (auto [r, c] : grid.find_all(ch)) {
        result += 100 * r + c;
    }
    return result;
}

bool do_move(Point pos, Point dir, StringGrid& grid) {
    auto np = pos + dir;
    if (grid[np] == '#') return false;
    if (grid[np] == 'O') {
        if (!do_move(np, dir, grid)) return false;
    }
    assert(grid[np] == '.');
    grid[np] = grid[pos];
    grid[pos] = '.';
    return true;
}

long one(StringGrid& grid, const string& moves) {
    auto pos = grid.find_all('@')[0];
    cout << "Robot starts at: " << pos << endl;
    for (auto m : moves) {
        // cout << "Move: " << m << endl;
        Point d = RC_DIRS.at(m);
        if (do_move(pos, d, grid)) pos = pos + d;
        // cout << grid << endl;
    }
    // cout << "final grid:" << grid << endl;
    return total_gps(grid, 'O');
}

string expand_line(const string& line) {
    string result;
    for (auto c : line) {
        switch (c) {
            case '#':
                result.push_back('#');
                result.push_back('#');
                break;
            case 'O':
                result.push_back('[');
                result.push_back(']');
                break;
            case '.':
                result.push_back('.');
                result.push_back('.');
                break;
            case '@':
                result.push_back('@');
                result.push_back('.');
                break;
            default:
                assert(false);
        }
    }
    return result;
}

bool do_move2(Point pos, Point dir, StringGrid& grid, bool real = true) {
    if (grid[pos] == ']') pos = pos + RC_WEST;
    char me = grid[pos];
    assert(me == '[' || me == '@');

    auto np = pos + dir;
    auto np2 = np + RC_EAST;
    if (grid[np] == '#' || (me == '[' && grid[np2] == '#')) return false;

    if (me == '@') {
        if (grid[np] != '.' && !do_move2(np, dir, grid, real)) return false;
        if (real) {
            assert(grid[np] == '.');
            grid[np] = '@';
            grid[pos] = '.';
        }  
        return true;
    }

    // focus on []
    if (dir == RC_WEST) {
        if (grid[np] != '.' && !do_move2(np, dir, grid, real)) return false;
        if (real) {
            assert(grid[np] == '.');
            grid[np] = '[';
            grid[np2] = ']';
            grid[pos+RC_EAST] = '.';
        }
        return true;
    } else if (dir == RC_EAST) {
        if (grid[np2] != '.' && !do_move2(np2, dir, grid, real)) return false;
        if (real) {
            assert(grid[np2] == '.');
            grid[np2] = ']';
            grid[np] = '[';
            grid[pos] = '.';
        }
        return true;
    } else {    // NORTH or SOUTH treated the same way.
        if ((grid[np] != '.' && !do_move2(np, dir, grid, false)) ||
            (grid[np2] != '.' && !do_move2(np2, dir, grid, false))) return false;
        if (real) {
            if (grid[np] != '.') assert(do_move2(np, dir, grid, real));
            if (grid[np2] != '.') assert(do_move2(np2, dir, grid, real));
            assert(grid[np] == '.' && grid[np2] == '.');
            grid[np] = '[';
            grid[np2] = ']';
            grid[pos] = grid[pos+RC_EAST] = '.';
        }
        return true;
    }

    return false;
}

long two(StringGrid& grid, const string& moves) {
    auto pos = grid.find_all('@')[0];
    cout << "Robot starts at: " << pos << endl;
    for (auto m : moves) {
        // cout << "Move: " << m << endl;
        Point d = RC_DIRS.at(m);
        if (do_move2(pos, d, grid)) pos = pos + d;
        // cout << grid << "robot: " << pos << endl;
    }
    // cout << "final grid:" << grid << endl;
    return total_gps(grid, '[');
}

int main() {
    vector<string> lines;
    string moves;
    parse_input(lines, moves);
    // std::cout << lines << '\n' << moves << endl;
    StringGrid grid1(lines);
    cout << "grid1:" << grid1 << endl;
    print_answer("one", one(grid1, moves));

    vector<string> lines2;
    for (const auto& s : lines) lines2.push_back(expand_line(s));
    StringGrid grid2(lines2);
    cout << "grid2:" << grid2 << endl;
    print_answer("two", two(grid2, moves));
}