#include <algorithm>
#include <iterator>
#include <iostream>
#include <numeric>
#include <set>
#include <stdexcept>
#include <vector>

std::vector<long> read_input() {
    std::vector<long> v;
    std::copy(std::istream_iterator<long>(std::cin),
              std::istream_iterator<long>(),
              std::back_insert_iterator(v));
    return v;
}

long two(const std::vector<long>& nums) {
    auto sum = 0L;
    std::set seen{sum};
    while (true) {
        for (auto n : nums) {
            sum += n;
            if (seen.count(sum)) return sum;
            seen.insert(sum);
        }
    }
    // unreachable
    throw std::invalid_argument("failed to find running sums twice!");
}

int main() {
    auto nums = read_input();
    long one = std::accumulate(nums.cbegin(), nums.cend(), 0L);
    std::cout << "part one: " << one << std::endl;
    std::cout << "part two: " << two(nums) << std::endl;
}