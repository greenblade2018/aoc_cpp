#include "aoc.h"

using Orders = std::map<int, std::set<int>>;
using Updates = std::vector<std::vector<int>>;

Orders orders;
Updates updates;

void read_orders() {
    std::string line;
    while (true) {
        std::getline(std::cin, line);
        if (line.empty()) break;

        auto nums = ints(line);
        orders[nums[0]].insert(nums[1]);
    }
}

void read_updates() {
    std::string line;
    while (std::getline(std::cin, line)) {
        updates.push_back(ints(line));
    }
}

bool correct_order(const std::vector<int>& update) {
    for (size_t i = 1; i < update.size(); ++i) {
        if (!orders.count(update[i])) continue;
        for (size_t j = 0; j < i; ++j) {
            if (orders[update[i]].count(update[j])) return false;
        }
    }
    return true;
}

long one() {
    long result = 0;
    for (const auto& update : updates) {
        assert(update.size() % 2 == 1);
        if (correct_order(update)) {
            std::cout << "good: " << update << std::endl;
            result += update[update.size()/2];
        }
    }
    return result;
}

int reorder(std::vector<int>& update) {
    int n = 0;
    for (size_t i = 0; i < update.size(); ++i) {
        if (!orders.count(update[i])) continue;
        for (size_t j = 0; j < i; ++j) {
            if (orders[update[i]].count(update[j])) {
                std::swap(update[i], update[j]);
                ++n;
            }
        }
    }
    return n;
}

long two() {
    long result = 0;
    for (auto& update: updates) {
        if (reorder(update)) {
            std::cout << "reorder: " << update << std::endl;
            result += update[update.size()/2];
        }
    }
    return result;
}

int main() {
    read_orders();
    read_updates();
    std::cout << orders.size() << " orders:\n";
    std::cout << orders << std::endl;
    std::cout << updates.size() << " updates:\n";
    std::cout << updates << std::endl;

    print_answer("one", one());
    print_answer("two", two());
}