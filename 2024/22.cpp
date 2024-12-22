#include "aoc.h"
using namespace std;

static const long MOD = 16777216;

long mix_prune(long n, long m) {
    return (n ^ m) % MOD;
}

long gen_next(long secret) {
    secret = mix_prune(secret, secret << 6);
    secret = mix_prune(secret, secret >> 5);
    secret = mix_prune(secret, secret << 11);
    return secret;
}

long solve_one(const vector<long>& seeds) {
    long result = 0;
    for (const long s : seeds) {
        long r = s;
        for (int i = 0; i < 2000; ++i) r = gen_next(r);
        cout << format("{}: {}\n", s, r);
        result += r;
    }
    return result;
}

long parse_long(const string& s) {
    return stol(s);
}

using Trade = map<tuple<int,int,int,int>, int>;

void find_trade(long seed, Trade& trade) {
    trade.clear();
    vector<int> diffs;
    long prev = seed % 10;
    for (int i = 0; i < 2000; ++i) {
        seed = gen_next(seed);
        int d = seed % 10;
        diffs.push_back(d - prev);
        prev = d;
        if (diffs.size() >= 4) {
            size_t n = diffs.size() - 4;
            auto key = make_tuple(diffs[n], diffs[n+1], diffs[n+2], diffs[n+3]);
            if (!trade.count(key)) trade[key] = d;
        }
    }
}

void print_trade(const Trade& trade) {
    for (const auto& [t, v] : trade) {
        cout << format("<{},{},{},{}>: {}\n", get<0>(t), get<1>(t), get<2>(t), get<3>(t), v);
    }
}

long solve_two(const vector<long>& seeds) {
    vector<Trade> trades(seeds.size());
    for (size_t i = 0; i < seeds.size(); ++i) {
        find_trade(seeds[i], trades[i]);
    }

    long best = -1;
    for (int a = -9; a <= 9; ++a) {
        cout << "a = " << a << endl;
        for (int b = -9; b <= 9; ++b) {
            for (int c = -9; c <= 9; ++c) {
                for (int d = -9; d <= 9; ++d) {
                    auto seq = make_tuple(a, b, c, d);
                    long bananas = 0;
                    for (const auto& trade : trades) {
                        if (trade.count(seq)) bananas += trade.at(seq);
                    }
                    best = max(best, bananas);
                }
            }
        }
    }
    return best;
}

int main() {
    // long n= 123;
    // for (int i = 0; i < 10; ++i) {
    //     n = gen_next(n);
    //     cout << n << endl;
    // }

    auto seeds = read_input<long>(cin, parse_long);
    print_answer("one", solve_one(seeds));
    print_answer("two", solve_two(seeds));
}