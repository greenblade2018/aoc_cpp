#include <string>
#include <vector>
#include "aoc.h"
using namespace std;
using namespace aoc;

int main() {
    string s = "#1 @ 1,3: 4x4";
    auto v = ints(s);
    for (auto n : v) cout << n << endl;
    cout << v << endl;
}