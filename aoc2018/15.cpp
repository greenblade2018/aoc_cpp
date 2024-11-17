#include "aoc.h"
using namespace aoc;

struct Unit {
    Point pos;
    char race;
    int hp;
    int attack; 

    char enemy() const { return (race == 'G') ? 'E' : 'G'; }
};

std::ostream& operator<<(std::ostream& os, const Unit& unit) {
    return os << '{'
              << unit.pos << ", " << unit.race << ", "
              << unit.hp << ", " << unit.attack
              << '}';
}


class Game {
public:
    Game(const std::vector<std::string>& lines, int elf_attack);
    int play(); // Return number of full rounds played.
    bool play_round(); // Return false if game ends, i.e. round not full completed.
    bool game_over();
    std::vector<Unit> get_units();

private:
    Point adjacent_target(const Unit& me);
    bool attack(const Unit& me);
    std::pair<int, Point> move_to(const Unit& me);
    Point next_step(Point from, Point to, int dist);

private:
    StringGrid m_grid;
    std::map<Point, Unit> m_units;
    std::map<Point, Unit> m_next_units;
};

Game::Game(const std::vector<std::string>& lines, int elf_attack) : m_grid(lines) {
    for (char race : "GE") {
        int attack = (race == 'G') ? 3 : elf_attack;
        for (auto pos : m_grid.find_all(race)) {
            m_units[pos] = Unit{pos, race, 200, attack};
        }
    }
    // std::cout << "Game: " << m_units.size() << " units found:\n";
    // std::cout << m_units << std::endl;
}

std::vector<Unit> Game::get_units() {
    std::vector<Unit> result;
    result.reserve(m_units.size());
    for (auto it = m_units.cbegin(); it != m_units.cend(); ++it) {
        result.push_back(it->second);
    }
    return result;
}

int Game::play() {
    int round = 0;
    // std::cout << std::format("\n===== INITIAL STATE =====\n", round);
    // std::cout << m_grid << std::endl;
    // for (const auto& unit : m_units) std::cout << unit << std::endl;
    // std::cout << "===========================\n";

    while (play_round()) {
        ++round;

        // std::cout << std::format("\n===== AFTER ROUND {} =====\n", round);
        // std::cout << m_grid << std::endl;
        // for (const auto& unit : m_units) std::cout << unit << std::endl;
        // std::cout << "===========================\n";

    }
    return round;
}

bool Game::game_over() {
    if (m_units.empty()) return true;
    char race = m_units.begin()->second.race;
    for (auto it = m_units.cbegin(); it != m_units.cend(); ++it) {
        if (it->second.race != race) return false;
    }
    // std::cout << "Game over: " << race << " won!" << std::endl;
    return true;
}

bool Game::play_round() {
    m_next_units.clear();
    while (!m_units.empty()) {
        Unit me = m_units.begin()->second;
        m_units.erase(m_units.begin());

        // If no enemy found on the grid, game ends.
        if (m_grid.find_all(me.enemy()).empty()) {
            m_next_units[me.pos] = me;
            for (const auto& [pos, unit] : m_units) m_next_units[pos] = unit;
            std::swap(m_units, m_next_units);
            return false;
        }

        // First, try attacking adjacent enemies.
        if (attack(me)) {
            m_next_units[me.pos] = me;
            continue;
        }

        // Then, find all empty spaces in range of any emenies. Pick the one with shortest
        // path and in reading order.
        auto [td, tp] = move_to(me);
        // std::cout << me << ": move to " << tp << " in " << tp << " steps.\n";
        if (td < 0) {
            // no enmeny reachable - end my turn.
            m_next_units[me.pos] = me;
            continue;
        }

        Point new_pos = next_step(me.pos, tp, td);
        // std::cout << me << ": next step " << new_pos << std::endl;
        assert(new_pos.first >= 0 && new_pos.second >= 0);
        assert(m_grid[new_pos] == '.');
        m_grid[me.pos] = '.';
        me.pos = new_pos;
        m_grid[me.pos] = me.race;
        m_next_units[me.pos] = me;

        // Try attack adjacent enemies again.
        attack(me);
    }
    swap(m_units, m_next_units);

    return true;
}

// DON'T MOVE TO AN ENEMY. Move to an position in range to an enemy.
std::pair<int, Point> Game::move_to(const Unit& me) {
    const int INF = std::numeric_limits<int>::max();
    auto dists = m_grid.bfs(me.pos);
    std::pair<int, Point> result = {INF, {-1, -1}};
    for (Point enemy : m_grid.find_all(me.enemy())) {
        for (Point adj : m_grid.adjacents(enemy)) {
            if (m_grid[adj] != '.') continue;
            if (auto it = dists.find(adj); it != dists.end()) {
                result = std::min(result, std::make_pair(it->second, adj));
            }
        }
    }

    if (result.first == INF) result.first = -1;
    return result;
}

Point Game::next_step(Point from, Point to, int dist) {
    auto rev_dists = m_grid.bfs(to);
    for (Point adj : m_grid.adjacents(from)) {
        if (m_grid[adj] != '.') continue;
        if (rev_dists.count(adj) && rev_dists[adj] == dist - 1) return adj;
    }
    return Point{-1, -1};
}

Point Game::adjacent_target(const Unit& me) {
    std::pair<int, Point> result = {1000000, {-1, -1}};
    for (auto pos : m_grid.adjacents(me.pos)) {
        if (m_grid[pos] == me.enemy()) {
            assert(m_units.count(pos) || m_next_units.count(pos));
            int hp = m_units.count(pos) ? m_units[pos].hp : m_next_units[pos].hp;
            result = std::min(result, make_pair(hp, pos));
        }
    }
    return result.second;
}

bool Game::attack(const Unit& me) {
    auto pos = adjacent_target(me);
    // std::cout << me << ": adjacent target " << pos << std::endl;
    if (pos.first < 0) return false;

    auto& units = m_units.count(pos) ? m_units : m_next_units;
    assert(units.count(pos));
    units[pos].hp -= me.attack;
    // std::cout << me << ": did " << me.attack << " damage, target new hp = " << units[pos].hp << std::endl;
    if (units[pos].hp <= 0) {
        m_grid[pos] = '.';
        units.erase(pos);
    }

    return true;
}

long sum_hp(const std::vector<Unit>& units) {
    long total = 0;
    for (const auto& unit : units) {
        total += unit.hp;
    }
    return total;
}
long solve_one(const std::vector<std::string>& lines) {
    Game game(lines, 3);
    int round = game.play();
    return round * sum_hp(game.get_units());
}

int count_elf(const std::vector<Unit>& units) {
    int result = 0;
    for (const auto& unit : units) {
        if (unit.race == 'E') ++result;
    }
    return result;
}

long try_game(const std::vector<std::string>& lines, int elf_attack) {
    Game game(lines, elf_attack);
    int elf = count_elf(game.get_units());

    int round = game.play();
    auto units = game.get_units();
    if (elf != count_elf(units)) return -1;
    return round * sum_hp(units);
}

long solve_two(const std::vector<std::string>& lines) {
    int low = 3, high = 200;
    while (low + 1 < high) {
        if (int mid = (low + high) / 2; try_game(lines, mid) < 0) {
            low = mid;
        } else {
            high = mid;
        }
    }
    std::cout << "elf attack " << high << " works." << std::endl;
    return try_game(lines, high);
}
int main() {
    auto lines = read_inputs<std::string>(std::cin, take_string);
    print_answer("one", solve_one(lines));
    std::cout << "200 => " << try_game(lines, 200) << std::endl;
    print_answer("two", solve_two(lines));
}