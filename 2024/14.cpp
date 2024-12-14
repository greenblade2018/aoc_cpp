#include "aoc.h"

// const int WIDE = 11;
const int WIDE = 101;
const int HW = WIDE / 2;
// const int TALL = 7;
const int TALL = 103;
const int HT = TALL / 2;
using Robot = std::vector<int>;

int find_quad(int x, int y) {
    if (x < HW && y < HT) return 0;
    if (x > HW && y < HT) return 1;
    if (x < HW && y > HT) return 2;
    if (x > HW && y > HT) return 3;
    return -1;
}

void move(std::vector<Robot>& robots, int n) {
    for (auto& r : robots) {
        r[0] = (r[0] + n * r[2]) % WIDE;
        if (r[0] < 0) r[0] += WIDE;
        r[1] = (r[1] + n * r[3]) % TALL;
        if (r[1] < 0) r[1] += TALL;
    }

}

long solve_one(std::vector<Robot> robots) {
    move(robots, 100);
    std::vector<long> quads(4, 0);
    for (const auto& r : robots) {
        auto q = find_quad(r[0], r[1]);
        if (q >= 0) ++quads[q];
    }
    std::cout << "quads = " << quads << std::endl;
    return std::accumulate(quads.cbegin(), quads.cend(), 1L, std::multiplies<long>());
}

void print_robots(const std::vector<Robot>& robots, int round) {
    std::vector<std::string> buf(TALL, std::string(WIDE, ' '));
    for (const auto& r : robots) {
        int x = r[0], y = r[1];
        buf[y][x] = '*';
    }
    std::cout << std::format("\nRound {}:\n", round);
    for (const auto& line : buf) std::cout << line << std::endl;
}

int count_margin(const std::vector<Robot>& robots) {
    int result = 0;
    for (const auto& r : robots) {
        int x = r[0], y = r[1];
        if (x < HW - (y / 4) || x > HW + (y / 4)) ++result;
    }
    return result;
}

std::string to_string(const std::vector<Robot>& robots) {
    std::string result(TALL * WIDE, ' ');
    for (const auto& r : robots) {
        int x = r[0], y = r[1];
        result[y*TALL + x] = '*';
    }
    return result;
}

int solve_two(std::vector<Robot> robots) {
    std::map<std::string, int> visited;
    for (int r = 1; true; ++r) {
        move(robots, 1);
        std::cerr << r << std::endl;
        auto s = to_string(robots);
        if (visited.count(s)) {
            std::cerr << "BANG! " << r << std::endl;
            std::cerr << std::format("BANG! {} = {}\n", r, visited[s]);
            return r;
        }
        visited[s] = r;
        if ((r - 68) % 101 == 0) print_robots(robots, r);
    }

    return -1;
}

int main() {
    auto robots = read_input<Robot>(std::cin, ints);
    print_answer("one", solve_one(robots));
    print_answer("two", solve_two(robots));
}

// 68, 169, 270