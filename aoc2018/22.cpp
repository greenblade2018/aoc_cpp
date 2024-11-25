#include "aoc.h"

template<typename S>
struct Node {
    S state;
    long cost = 0;

    // Idiotically, std::priority_queue puts the largest key to the front.
    bool operator<(const Node& other) const { return cost > other.cost; }
};

template<typename S>
std::ostream& operator<<(std::ostream& os, const Node<S>& node) {
    return os << "Node(" << node.state << ", " << node.cost << ")";
}

constexpr long FAILURE_COST = std::numeric_limits<long>::max();

template<typename P, typename S>
Node<S> dijkstra_search(P& problem) {
    Node<S> node = {problem.initial_state(), 0};
    std::priority_queue<Node<S>> frontier;
    frontier.push(node);
    std::map<S, long> visited = {{node.state, 0}};

    while (!frontier.empty()) {
        node = frontier.top();  // reuse the node object
        frontier.pop();
        // std::cout << node << std::endl;
        if (problem.is_goal(node.state)) return node;

        for (const auto& [new_state, cost] : problem.actions(node.state)) {
            auto new_cost = node.cost + cost;
            if (!visited.count(new_state) || visited[new_state] > new_cost) {
                visited[new_state] = new_cost;
                frontier.emplace(new_state, new_cost);
            }
        }
    }
    return Node{S(), FAILURE_COST};
}

const int ROCKY = 0, WET = 1, NARROW = 2;
const int NONE = 0, TORCH = 1, CLIMB = 2;
const std::string GEAR_STR[] = {"None", "Torch", "Climb"};

class Cave {
public:
    using State = std::pair<Point, int>;
    using Action = std::pair<State, long>;

public:
    Cave(long depth, Point target);
    long rect_risk();
    State initial_state() const { return State{{0, 0}, TORCH}; }
    bool is_goal(const State& state) const { return state.first == m_target && state.second == TORCH; }
    std::vector<Action> actions(const State& state);

private:
    long get_geo(Point p);
    long get_ero(Point p);
    long get_risk(Point p);

private:
    static const long MOD = 20183;

    long m_depth;
    Point m_target;
    std::map<Point, long> m_geo;
    std::map<Point, long> m_ero;
    std::map<Point, long> m_risk;
};

std::ostream& operator<<(std::ostream& os, const Cave::State& s) {
    return os << "State(" << s.first << ", " << GEAR_STR[s.second] << ")";
}

Cave::Cave(long depth, Point target) : m_depth(depth), m_target(target) {
    Point zero(0, 0);
    m_geo[zero] = m_geo[target] = 0;
}

long Cave::rect_risk() {
    auto [X, Y] = m_target;
    long result = 0;
    for (int x = 0; x <= X; ++x) {
        for (int y = 0; y <= Y; ++y) result += get_risk(Point(x, y));
    }
    return result;
}

std::vector<Cave::Action> Cave::actions(const State& state) {
    std::vector<Action> results;
    auto [pos, gear] = state;
    // First, we can move to adjacent regions.
    // NOTE:
    // 1. we can not goto negative coodinates.
    // 2. The new cell must be compatible with current gear.
    for (const auto& [_, d] : DIRS) {
        auto new_pos = pos + d;
        if (new_pos.first < 0 || new_pos.second < 0) continue;
        if (get_risk(new_pos) == ROCKY && gear == NONE) continue;
        if (get_risk(new_pos) == WET && gear == TORCH) continue;
        if (get_risk(new_pos) == NARROW && gear == CLIMB) continue;
        results.emplace_back(make_pair(new_pos, gear), 1);
    }
    // Then try to change gear
    switch (get_risk(pos)) {
        case ROCKY:
            results.emplace_back(make_pair(pos, TORCH), 7);
            results.emplace_back(make_pair(pos, CLIMB), 7);
            break;
        case WET:
            results.emplace_back(make_pair(pos, CLIMB), 7);
            results.emplace_back(make_pair(pos, NONE), 7);
            break;
        case NARROW:
            results.emplace_back(make_pair(pos, TORCH), 7);
            results.emplace_back(make_pair(pos, NONE), 7);
            break;
        default:
            std::cerr << "pos = " << pos << ", risk = " << get_risk(pos) << std::endl;
            assert(false);
    }
    return results;
}

long Cave::get_risk(Point p) {
    if (!m_risk.count(p)) m_risk[p] = get_ero(p) % 3;
    return m_risk[p];
}

long Cave::get_ero(Point p) {
    if (!m_ero.count(p)) m_ero[p] = (m_depth + get_geo(p)) % MOD;
    return m_ero[p];
}

long Cave::get_geo(Point p) {
    if (!m_geo.count(p)) {
        auto [x, y] = p;
        if (y == 0) {
            m_geo[p] = ((long)x * 16807) % MOD;
        } else if (x == 0) {
            m_geo[p] = ((long)y * 48271) % MOD;
        } else {
            m_geo[p] = (get_ero(Point(x-1, y)) * get_ero(Point(x, y-1))) % MOD;
        }
    }
    return m_geo[p];
}



int main() {
    auto input = read_input<std::vector<int>>(std::cin, ints);
    Cave cave(input[0][0], Point(input[1][0], input[1][1]));
    print_answer("one", cave.rect_risk());
    auto node = dijkstra_search<Cave, Cave::State>(cave);
    print_answer("two", node.cost);
}