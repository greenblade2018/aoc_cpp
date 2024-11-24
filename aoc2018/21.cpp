#include "aoc.h"
#include "device.h"


long solve_one(Device& dev) {
    dev.reset();
    dev.set_bp(28); // stop after 'r3 = r1 ==r0;'
    dev.run();
    return dev.reg(1);
}

long solve_two(Device& dev) {
    dev.reset();
    dev.clear_bp();
    dev.set_bp(28);

    std::set<long> visited;
    long prev;
    while (true) {
        dev.run();
        auto n = dev.reg(1);
        std::cout << visited.size() << ' ' << n << std::endl;
        if (visited.count(n)) return prev;
        visited.insert(n);
        prev = n;
    }
    // unreachable
    return -1;
}

int main() {
    auto dev = Device::parse(std::cin);
    print_answer("one", solve_one(dev));
    print_answer("two", solve_two(dev));
}