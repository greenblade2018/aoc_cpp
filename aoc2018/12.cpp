#include "aoc.h"


using Rules = std::map<std::string, char>;

std::pair<std::string, Rules> parse_inputs(std::istream& is) {
    std::string line;
    std::getline(is, line);
    std::string initial = line.substr(15);

    std::getline(is, line);
    Rules rules;
    while (getline(is, line)) {
        auto k = line.substr(0, 5);
        auto v = line[9];
        rules[k] = v;
    }

    return std::make_pair(initial, rules);
}

std::set<long> spread(const Rules& rules, const std::set<long>& plants) {
    long low = *plants.cbegin() - 2;
    long high = *plants.crbegin() + 2;
    std::set<long> result;
    for (long n = low; n <= high; ++n) {
        std::string key(5, '.');
        for (int i = -2; i <= 2; ++i) {
            if (plants.count(n + i)) key[i+2] = '#';
        }

        // std::cerr << "key=" << key << std::endl;
        if (rules.at(key) == '#') result.insert(n);
    }
    return result;
}

long solve_one(std::set<long> plants, const Rules& rules, int generation) {
    while (generation-- > 0) {
        plants = spread(rules, plants);
    }
    return std::accumulate(plants.cbegin(), plants.cend(), 0L);
}

long solve_two(std::set<long> plants, const Rules& rules, long generation) {
    long n = 200;
    std::vector<long> v(n);
    for (long i = 0; i < n; ++i) {
        plants = spread(rules, plants);
        v[i] = std::accumulate(plants.cbegin(), plants.cend(), 0L);
    }

    long result = v[n-1] + ((generation - n) * (v[n-1] - v[n-2]));
    return result;
}


int main() {
    auto [initial, rules] = parse_inputs(std::cin);
    std::cout << "initial = " << initial << std::endl;
    std::cout << "rules   = " << rules << std::endl;

    std::set<long> plants;
    for (size_t i = 0; i < initial.size(); ++i) {
        if (initial[i] == '#') plants.insert((long)i);
    }

    print_answer("one", solve_one(plants, rules, 20));
    print_answer("two", solve_two(plants, rules, 50'000'000'000L));
}