#include "aoc.h"
#include "device.h"
using namespace aoc;

int solve_one(Device& dev) {
    dev.reset();
    dev.run(1);
    return dev.reg(0);
}

int factor_sum(int n) {
    int result = 0;
    for (int i = 1; i <= n; ++i) {
        if (n % i == 0) result += i;
    }
    return result;
}

int main() {
    // print_answer("one", factor_sum(947));
    // print_answer("two", factor_sum(947 + 10550400));
    auto dev = Device::parse(std::cin);
    print_answer("one", solve_one(dev));
}