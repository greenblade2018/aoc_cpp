#include "aoc.h"
#include <cstdlib>
using namespace std;

using Wire = pair<int, vector<string>>; // value, formula
using Graph = map<string, set<string>>;

map<string, Wire> wires;
Graph graph;
vector<string> order;
int nx = 0, ny = 0;

long calculate() {
    for (const auto& s : order) {
        auto& [value, form] = wires[s];
        if (!form.empty()) {
            // assert(value == -1 && form.size() == 3);
            string a = form[0], op = form[1], b = form[2];
            int va = wires[a].first, vb = wires[b].first;
            assert(va >= 0 && vb >= 0);
            if (op == "AND") value = va & vb;
            else if (op == "OR") value = va | vb;
            else if (op == "XOR") value = va ^ vb;
            else assert(false);
        }
    }

    long one = 0;
    for (auto it = wires.crbegin(); it != wires.crend(); ++it) {
        if (it->first[0] != 'z') break;
        one = one * 2 + it->second.first;
    }
    return one;
}

long gen_random(long upper) {
    long n = ((long)rand() << 31) | (long)rand();
    return n % upper;
}

int test_calc(int repeat) {
    int correct = 0;
    while (repeat-- > 0) {
        long x = gen_random(1L << nx), y = gen_random(1L << ny);
        long save_x = x, save_y = y;
        string w = "x00";
        for (int i = 0; i < nx; ++i) {
            wires[w].first = x & 1;
            x /= 2;
            if (w[2] == '9') {
                w[2] = '0';
                ++w[1];
            } else {
                ++w[2];
            }
        }
        w = "y00";
        for (int i = 0; i < ny; ++i) {
            wires[w].first = y & 1;
            y /= 2;
            if (w[2] == '9') {
                w[2] = '0';
                ++w[1];
            } else {
                ++w[2];
            }
        }

        long z = calculate();
        // std::cout << format("{} op {} => {}\n", save_x, save_y, z);
        if (z == (save_x + save_y)) ++correct;
    }
    return correct;
}

void swap_wires(const string& a, const string& b) {
    swap(wires[a], wires[b]);
    auto ia = find(order.begin(), order.end(), a), ib = find(order.begin(), order.end(), b);
    assert(ia != order.end() && ib != order.end());
    swap(*ia, *ib);
}

string vs_to_string(vector<string>& v) {
    sort(v.begin(), v.end());
    string s = v[0];
    for (size_t i = 1; i < v.size(); ++i) {
        s += ",";
        s += v[i];
    }
    return s;
}

// z06 - fkp, z11 - ngr, z31 - mfm, z45

string solve_two() {
    vector<string> vars;
    for (const auto& p : wires) {
        if (p.first[0] != 'x' && p.first[0] != 'y') vars.push_back(p.first);
    }
    std::cout << "vars: " << vars << endl;

    vector<string> sel{"z06", "fkp", "z11", "ngr", "z31", "mfm"};
    string answer = "NOT FOUND!";
    for (string a : vars) {
        if (find(sel.begin(), sel.end(), a) != sel.end()) continue;
        sel.push_back(a);
        for (string b : vars) {
            if (find(sel.begin(), sel.end(), b) != sel.end()) continue;
            sel.push_back(b);
            cout << "sel: " << sel << endl;
            for (size_t i = 0; i < sel.size(); i += 2) swap_wires(sel[i], sel[i+1]);
            if (test_calc(10) >= 9) return vs_to_string(sel);
            for (size_t i = 0; i < sel.size(); i += 2) swap_wires(sel[i], sel[i+1]);
            sel.pop_back();
        }
        sel.pop_back();
    }

    return answer;
}

int main() {
    string line;
    map<string, int> degree;

    while(true) {
        getline(cin, line);
        if (line.empty()) break;
        string s = line.substr(0, 3);
        wires[s] = {(int)(line[5] - '0'), {}};
        if (s[0] == 'x') ++nx;
        if (s[0] == 'y') ++ny;
    }
    std::cout << format("nx = {}, ny = {}\n", nx, ny);

    while (getline(cin, line)) {
        auto sep1 = line.find(' ');
        auto sep2 = line.find(' ', sep1 + 1);
        auto sep3 = line.find(" -> ");
        assert(sep1 != string::npos && sep2 != string::npos && sep3 != string::npos);
        string a = line.substr(0, 3), op = line.substr(sep1 + 1, sep2 - sep1 - 1), b = line.substr(sep2 + 1, 3);
        string c = line.substr(line.size() - 3, 3);
        graph[a].insert(c);
        graph[b].insert(c);
        wires[c] = {-1, {a, op, b}};
        degree[c] += 2;
    }
    std::cout << "wires: " << wires << endl;
    std::cout << "graph: " << graph << endl;
    std::cout << "degree: " << degree << endl;

    queue<string> que;
    for (const auto& p : wires) {
        if (degree[p.first] == 0) que.push(p.first);
    }

    while (!que.empty()) {
        auto s = que.front();
        que.pop();
        order.push_back(s);

        for (auto u : graph[s]) {
            --degree[u];
            if (degree[u] == 0) que.push(u);
        }
    }
    std::cout << "order: " << order << endl;

    print_answer("one", calculate());
    print_answer("two", solve_two());
}