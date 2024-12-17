#include "aoc.h"
using namespace std;

using State = pair<Point, Point>;   // position, direction

class MazeProblem : public SearchProblem<State> {
public:
    MazeProblem(const vector<string>& grid);

    State initial_state() override { return {m_start, RC_EAST}; }
    bool is_goal(const State& state) override { return state.first == m_end; }
    vector<Action> actions(const State& state) override;
    long hfunc(const State& state) override { return taxi_distance(state.first, m_end); }

private:
    StringGrid m_grid;
    Point m_start;
    Point m_end;
};

MazeProblem::MazeProblem(const vector<string>& grid) : m_grid(grid) {
    m_start = m_grid.find_all('S').at(0);
    m_end = m_grid.find_all('E').at(0);
    cout << "start = " << m_start << ", end = " << m_end << endl;
}

vector<MazeProblem::Action> MazeProblem::actions(const State& state) {
    vector<Action> results;
    auto [pos, dir] = state;
    auto np = pos + dir;
    if (m_grid.in_range(np) && m_grid[np] != '#') results.emplace_back(State{np, dir}, 1);

    results.emplace_back(State{pos, rc_make_turn(dir, 'L')}, 1000);
    results.emplace_back(State{pos, rc_make_turn(dir, 'R')}, 1000);
    return results;
}

using Node = pair<int, State>;

pair<int, int> dijkstra(const StringGrid& grid) {
    Point start = grid.find_all('S').at(0);
    Point end = grid.find_all('E').at(0);

    State src{start, RC_EAST};
    map<State, set<Point>> seats;
    seats[src].insert(start);
    map<State, int> costs;
    costs[src] = 0;
    priority_queue<Node> que;
    que.emplace(0, src);

    while (!que.empty()) {
        auto [cost, state] = que.top();
        cost = -cost;
        que.pop();
        assert(costs.count(state));
        auto [pos, dir] = state;
        if (costs[state] < cost) continue;

        vector<Node> actions;
        auto np = pos + dir;
        if (grid.in_range(np) && grid[np] != '#') {
            actions.emplace_back(cost + 1, State{np, dir});
        }
        for (char ch : "LR") {
            auto nd = rc_make_turn(dir, ch);
            actions.emplace_back(cost + 1000, State{pos, nd});
        }

        for (const auto& [nc, ns] : actions) {
            // cout << "checking " << nc << ' ' << ns << endl;
            if (!costs.count(ns) || costs[ns] > nc) {
                que.push(Node{-nc, ns});
                costs[ns] = nc;
                seats[ns] = seats[state];
                seats[ns].insert(ns.first);
            }
            else if (costs[ns] == nc) {
                // cout << "merging " << nc << ' ' << ns << endl;
                for (const auto& x : seats[state]) seats[ns].insert(x);
            }
            else {
                // cout << "discarding " << nc << ' ' << ns << endl;
            }
        }
    }

    int best = 1'000'000'000;
    for (const auto& p : RC_DIRS) {
        best = min(best, costs[State{end, p.second}]);
    }
    set<Point> best_seats;
    for (const auto& p : RC_DIRS) {
        State s{end, p.second};
        if (costs[s] == best) {
            for (const auto& x : seats[s]) best_seats.insert(x);
        }
    }
    // cout << best << ' ' << best_seats << endl;
    return {best, (int)best_seats.size()};
}

int main() {
    auto lines = read_input<string>(cin, take_string);
    // MazeProblem maze(lines);
    // auto node = a_star_search(maze);
    // print_answer("one", node.cost);
    StringGrid grid(lines);
    auto [one, two] = dijkstra(grid);
    print_answer("one", one);
    print_answer("two", two);
}