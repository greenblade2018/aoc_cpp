#include "aoc.h"
using namespace std;

using Graph = map<string, set<string>>;
Graph graph;

string calc_sig(string u, string v, string w) {
    vector<string> tmp{u, v, w};
    sort(tmp.begin(), tmp.end());
    return tmp[0] + "," + tmp[1] + "," + tmp[2];
}

int main() {
    string line;
    while(getline(cin, line)) {
        string u = line.substr(0, 2), v = line.substr(3, 2);
        graph[u].insert(v);
        graph[v].insert(u);
    }

    set<string> result;
    for (const auto& [u, adj] : graph) {
        for (const auto& v : adj) {
            if (v == u) continue;
            for (const auto& w : graph[v]) {
                if (w == u || w == v) continue;
                if (adj.count(w)) result.insert(calc_sig(u, v, w));
            }
        }
    }
    cout << "result = " << result << ' ' << result.size() << endl;
    int one = count_if(result.cbegin(), result.cend(),
                       [](const string& s) { return s[0] == 't' || s.find(",t") != string::npos; });
    print_answer("one", one);
}