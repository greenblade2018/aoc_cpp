#include <format>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;

int main() {
    for (int i = 0; i < 3; ++i) {
        vector<int> v(5, -2);
        for (int n : v) std::cout << n << ' '; cout << endl;
        v[1] = 800;
        for (int n : v) std::cout << n << ' '; cout << endl;
    }
}