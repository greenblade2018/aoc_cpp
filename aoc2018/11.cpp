#include "aoc.h"
using namespace aoc;

long cells[301][301];

long calc_power(int x, int y, long serial) {
    long power = (x + 10) * y + serial;
    power *= x + 10;
    power = (power % 1000) / 100;
    power -= 5;
    return power;
}

void init_cells(long serial) {
    for (int y = 1; y <= 300; ++y) {
        for (int x = 1; x <= 300; ++x) {
            cells[y][x] = calc_power(x, y, serial);
        }
    }
}

Point solve_one() {
    long best = std::numeric_limits<long>::min();
    int best_x = -1, best_y = -1;
    for (int y0 = 1; y0 <= 300 - 2; ++y0) {
        for (int x0 = 1; x0 <= 300 - 2; ++x0) {
            long sum = 0;
            for (int y = y0; y < y0 + 3; ++y) {
                for (int x = x0; x < x0 + 3; ++x) sum += cells[y][x];
            }
            if (sum > best) {
                best = sum;
                best_x = x0;
                best_y = y0;
            }
        }
    }
    return Point(best_x, best_y);
}

std::pair<Point, int> solve_two() {
    long best = std::numeric_limits<long>::min();
    int best_x = -1, best_y = -1, best_z = -1;
    for (int y0 = 1; y0 <= 300; ++y0) {
        for (int x0 = 1; x0 <= 300; ++x0) {
            for (int z = 1; x0 + z <= 301 && y0 + z <= 301; ++z) {
                long sum = 0;
                for (int y = y0; y < y0 + z; ++y) {
                    for (int x = x0; x < x0 + z; ++x) sum += cells[y][x];
                }
                if (sum > best) {
                    best = sum;
                    best_x = x0;
                    best_y = y0;
                    best_z = z;
                }
            }
        }
    }
    return make_pair(Point(best_x, best_y), best_z);
}

int main() {
    assert(calc_power(122, 79, 57) == -5);
    assert(calc_power(217, 196, 39) == 0);
    assert(calc_power(101, 153, 71) == 4);

    init_cells(7400);
    print_answer("one", solve_one());
    print_answer("two", solve_two());
}