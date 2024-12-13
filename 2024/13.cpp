#include "aoc.h"

using Record = std::vector<std::vector<long>>;
const long INF = std::numeric_limits<long>::max();

long play(const Record& record) {
    long best = INF;
    for (int a = 0; a <= 100; ++a) {
        long tmp = record[2][0] - record[0][0] * a;
        if (tmp < 0) break;
        if (tmp % record[1][0] != 0) continue;
        long b = tmp / record[1][0];
        if (record[0][1] * a + record[1][1] * b != record[2][1]) continue;
        long token = 3 * a + b;
        best = std::min(best, token);
    }

    if (best == INF) {
        std::cout << record << " => IMPOSSIBLE!\n";
    } else {
        std::cout << record << " => " << best << std::endl;
    }
    return best == INF ? 0 : best;
}

// We have:
// a * x0 + b * x1 = x, and
// a * y0 + b * y1 = y. Find a, b.
// a*x0*y0 + b*x1*y0 = x*y0
// a*x0*y0 + b*x0*y1 = y*x0  ==> (x1*y0 - x0*y1) * b = x*y0 - y*x0;
long play2 (const Record& r) {
    long tmp1 = r[1][0] * r[0][1] - r[0][0] * r[1][1];
    long tmp2 = r[2][0] * r[0][1] - r[2][1] * r[0][0];
    if (tmp1 == 0 || std::abs(tmp2) % std::abs(tmp1) != 0) return 0;
    long b = tmp2 / tmp1;
    if (b < 0) return 0;

    long tmp3 = r[2][0] - b * r[1][0];
    if (tmp3 < 0 || tmp3 % r[0][0] != 0) return 0;
    long a = tmp3 / r[0][0];

    assert(a * r[0][1] + b * r[1][1] == r[2][1]);
    return 3 * a + b;
}


long solve_one(const std::vector<Record>& records) {
    long result = 0;
    for (const auto& record : records) result += play2(record);
    return result;
}

long solve_two(std::vector<Record>& records) {
    long result = 0;
    for (auto& r : records) {
        r[2][0] += 10000000000000L;
        r[2][1] += 10000000000000L;
        result += play2(r);
    }
    return result;
}

int main() {
    std::string line;
    std::vector<Record> records;

    while (std::getline(std::cin, line)) {
        Record r;
        r.push_back(longs(line));
        std::getline(std::cin, line);
        r.push_back(longs(line));
        std::getline(std::cin, line);
        r.push_back(longs(line));
        records.push_back(r);

        std::getline(std::cin, line);   // read the empty line
    }
    // for (const auto& r : records) std::cout << r << std::endl;

    print_answer("one", solve_one(records));
    print_answer("two", solve_two(records));
}