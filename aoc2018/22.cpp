#include "aoc.h"

constexpr long MOD = 20183;
long depth;
int X, Y;
long geo[1024][1024];
long ero[1024][1024];

long solve_one() {
    for (int x = 0; x <= X; ++x) {
        geo[x][0] = ((long)x * 16807) % MOD;
        ero[x][0] = (depth + geo[x][0]) % MOD;
    }
    for (int y = 0; y <= Y; ++y) {
        geo[0][y] = ((long)y * 48271) % MOD;
        ero[0][y] = (depth + geo[0][y]) % MOD;
    }

    for (int x = 1; x <= X; ++x) {
        for (int y = 1; y <= Y; ++y) {
            geo[x][y] = (ero[x][y-1] * ero[x-1][y]) % MOD;
            ero[x][y] = (depth + geo[x][y]) % MOD;
        }
    }
    geo[X][Y] = 0;
    ero[X][Y] = (depth + geo[X][Y]) % MOD;

    long result = 0;
    for (int x = 0; x <= X; ++x) {
        for (int y = 0; y <= Y; ++y) result += ero[x][y] % 3;
    }
    return result;
}

int main() {
    auto input = read_input<std::vector<int>>(std::cin, ints);
    depth = input[0][0];
    X = input[1][0];
    Y = input[1][1];
    std::cout << depth << ' ' << X << ' ' << Y << std::endl;

    print_answer("one", solve_one());
}