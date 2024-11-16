#include "aoc.h"
using namespace aoc;

using Grid = std::vector<std::string>;

class Cart {
public:
    static const std::string TURNS;
    friend std::ostream& operator<<(std::ostream& os, const Cart& car);
public:
    Cart() {}
    Cart(Point pos, Point dir): m_pos{pos}, m_dir{dir}, m_idx{0} {}
    Point position() const { return m_pos; }
    void move(const Grid& grid);
private:
    Point m_pos = {0, 0};
    Point m_dir = {0, 0};
    size_t m_idx = 0;
};

const std::string Cart::TURNS = "L#R";

std::ostream& operator<<(std::ostream& os, const Cart& car) {
    return os << '{' << car.m_pos << ", " << car.m_dir << ", " << Cart::TURNS[car.m_idx] << '}';
}

void Cart::move(const Grid& grid) {
    auto [x, y] = m_pos;
    char turn = '#';
    switch (grid[y][x]) {
        case '/':
            turn = (m_dir == EAST || m_dir == WEST) ? 'L' : 'R';
            break;
        case '\\':
            turn = (m_dir == EAST || m_dir == WEST) ? 'R' : 'L';
            break;
        case '+':
            turn = TURNS[m_idx];
            m_idx = (m_idx + 1) % TURNS.size();
            break;
        default:
            // do nothing
            break;
    }
    m_dir = make_turn(m_dir, turn);
    m_pos = m_pos + m_dir;
}

// Use map to store carts so that they're always sorted by positions.
using Carts = std::map<Point, Cart>;

Point solve_one(const Grid& grid, Carts carts) {
    while (true) {
        assert(carts.size() >= 2);
        Carts new_carts;
        while (!carts.empty()) {
            Cart car = carts.begin()->second;
            carts.erase(carts.begin());

            car.move(grid);
            auto pos = car.position();
            if (carts.count(pos) || new_carts.count(pos)) return pos;
            new_carts[pos] = car;
        }
        std::swap(carts, new_carts);
    }
    // unreachable
    return Point{-1, -1};
}

Point solve_two(const Grid& grid, Carts carts) {
    while (carts.size() > 1) {
        Carts new_carts;
        while (!carts.empty()) {
            auto it = carts.cbegin();
            auto car = it->second;
            carts.erase(it);

            car.move(grid);
            Point pos = car.position();
            if (auto it = carts.find(pos); it != carts.end()) {
                carts.erase(it);
                continue;
            }
            if (auto it = new_carts.find(pos); it != new_carts.end()) {
                new_carts.erase(it);
                continue;
            }
            new_carts[pos] = car;
        }
        swap(carts, new_carts);
    }

    return carts.cbegin()->first;
}

int main() {
    auto lines = read_inputs<std::string>(std::cin, take_string);
    Carts carts;
    for (size_t y = 0; y < lines.size(); ++y) {
        for (size_t x = 0; x < lines[0].size(); ++x) {
            char c = lines[y][x];
            if (auto it = DIRS.find(c); it != DIRS.end()) {
                lines[y][x] = (c == '^' || c == 'v') ? '|' : '-';
                Point pos(x, y);
                carts[pos] = Cart{pos, it->second};
            }
        }
    }
    std::cout << carts.size() << " carts: " << carts << std::endl;
    Grid grid(lines);

    print_answer("one", solve_one(grid, carts));
    print_answer("two", solve_two(grid, carts));
}