#include "aoc.h"
using namespace std;

using Graph = map<string, set<string>>;
Graph graph;

string calc_sig(string u, string v, string w) {
    vector<string> tmp{u, v, w};
    sort(tmp.begin(), tmp.end());
    return tmp[0] + "," + tmp[1] + "," + tmp[2];
}

set<string> intersect(const set<string>& a, const set<string>& b) {
    set<string> c;
    for (const auto& x : a) {
        if (b.count(x)) c.insert(x);
    }
    return c;
}

set<string> clique;

void BK(set<string> R, set<string> P, set<string> X) {
    if (P.empty() && X.empty()) {
        cout << "Maximal clique: " << R << endl;
        if (R.size() > clique.size()) clique = R;
        return;
    }

    for (auto it = P.begin(); it != P.end(); /*NONE*/) {
        auto v = *it;
        auto NR = R;
        NR.insert(v);
        auto NP = intersect(P, graph[v]);
        auto NX = intersect(X, graph[v]);
        BK(NR, NP, NX);

        it = P.erase(it);
        X.insert(v);
    }
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

    set<string> all;
    for (const auto& p : graph) all.insert(p.first);
    cout << "all: " << all << endl;
    BK(set<string>(), all, set<string>());
    cout << "part two:\n";
    for (auto it = clique.begin(); it != clique.end(); ++it) {
        if (it != clique.begin()) cout << ',';
        cout << *it;
    }
    cout << endl;
}