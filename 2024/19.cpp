#include "aoc.h"
using namespace std;

vector<string> patterns, designs;

void parse_input() {
    string line;
    getline(cin, line);
    patterns = words(line);

    getline(cin, line);
    while (getline(cin, line)) designs.push_back(line);

    cout << "patterns: " << patterns << endl;
    cout << "designs: " << designs << endl;
}

// dp[i]: number of ways to make design at position i.
long dp[1024];

long do_design(const string& design) {
    memset(dp, 0, sizeof(dp));
    dp[0] = 1;

    for (size_t i = 0; i < design.size(); ++i) {
        if (!dp[i]) continue;
        for (const auto& p : patterns) {
            auto s = design.substr(i, p.size());
            if (s == p) dp[i+p.size()] += dp[i];
        }
    }
    return dp[design.size()];
}

pair<long, long> solve() {
    long one = 0, two = 0;
    for (const auto& d : designs) {
        auto ok = do_design(d);
        cout << d << " => " << ok << endl;
        if (ok) ++one;
        two += ok;
    }
    return {one, two};
}

int main() {
    parse_input();
    auto [one, two] = solve();
    print_answer("one", one);
    print_answer("two", two);
}