#include "aoc.h"
using namespace std;

std::vector<std::string> alphanums(const std::string& s) {
    std::regex pattern(R"([a-zA-z0-9]+)");
    std::vector<std::string> ret;
    std::transform(std::sregex_iterator(s.cbegin(), s.cend(), pattern),
        std::sregex_iterator(),
        std::back_inserter(ret),
        [](const auto& mr) { return mr.str(); });
    return ret;

}

vector<vector<string>> gates;

int find_target(const string& in, const string& op) {
    for (int i = 0; i < (int)gates.size(); ++i) {
        const auto& g = gates[i];
        if (g[1] == op && (g[0] == in || g[2] == in)) return i;
    }

    return -1;
}

int main() {
    gates = read_input<vector<string>>(cin, alphanums);

    vector<string> r_xor(50), r_and(50);
    string x{"x00"};
    for (int i = 0; i < 45; ++i) {
        x[1] = '0' + (i / 10);
        x[2] = '0' + (i % 10);
        int d = find_target(x, "XOR");
        int c = find_target(x, "AND");
        cout << gates[d] << "; " << gates[c] << "; ";
        
        int z = find_target(gates[d][3], "XOR");
        if (z >= 0) cout << gates[z] << "; ";
        else cout << "####################; ";

        int cc = find_target(gates[d][3], "AND");
        if (cc >= 0) {
            cout << gates[cc] << "; ";
            int n = find_target(gates[cc][3], "OR");
            if (n >= 0) cout << gates[n];
        } else {
            cout << "####################; ";
        }
        cout << endl;
    }

    vector<string> ret{"z11", "z31", "z06", "ngr", "mfm", "fkp", "bpt", "krj"};
    sort(ret.begin(), ret.end());
    cout << ret[0];
    for (size_t i = 1; i < ret.size(); ++i) cout << "," << ret[i];
    cout << endl;
}

// z11 - ngr?
// z31 - mfm?
// z38 - fkp
// z06
// krj - bpt

// z06
// z11
// z31
// fkp
// ngr
// mfm
