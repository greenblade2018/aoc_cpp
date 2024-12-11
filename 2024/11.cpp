#include "aoc.h"

size_t blink(const std::vector<long>& init, int n) {
    std::list<long> stones(init.begin(), init.end());
    for (int i = 0; i < n; ++i) {
        for (auto it = stones.begin(); it != stones.end(); ++it) {
            if (*it == 0) {
                *it = 1;
            }
            else {
                auto s = std::to_string(*it);
                if (s.size() % 2 == 0) {
                    auto m = s.size() / 2;
                    long a = std::stol(s.substr(0, m));
                    long b = std::stol(s.substr(m));
                    stones.insert(it, a);
                    *it = b;
                } else {
                    *it *= 2024;
                }

            }
        }

        std::cout << std::format("blink {}: {}\n", i + 1, stones.size());
        // std::cout << std::format("blink {}:", i + 1);
        // for (auto it = stones.cbegin(); it != stones.cend(); ++it) std::cout << ' ' << *it;
        // std::cout << std::endl;
    }
    return stones.size();
}

const int MAXN = 10000;
size_t cache[MAXN][76];     // default initialized to 0

size_t dp(long n, int k) {
    if (n < MAXN && cache[n][k] > 0) return cache[n][k];

    size_t result = 0;
    if (k == 0) {
        result = 1;
    }
    else if (n == 0) {
        result = dp(1, k-1);
    }
    else {
        auto s = std::to_string(n);
        if (s.size() % 2 == 0) {
            auto m = s.size() / 2;
            long a = std::stol(s.substr(0, m));
            long b = std::stol(s.substr(m));
            result = dp(a, k - 1) + dp(b, k - 1);
        } else {
            result = dp(n * 2024, k - 1);
        }
    }

    if (n < MAXN) cache[n][k] = result;
    return result;
}

size_t solve(const std::vector<long>& nums, int k) {
    size_t result = 0;
    for (auto n : nums) {
        result += dp(n, k);
    }
    return result;
}

int main() {
    auto stones = read_input<std::vector<long>>(std::cin, longs)[0];
    std::cout << "stones = " << stones << std::endl;
    print_answer("one", solve(stones, 25));
    print_answer("two", solve(stones, 75));
}