#include "aoc.h"

using Graph = std::map<Point, std::vector<Point>>;
using Positions = std::set<Point>;

Positions _build_graph(const Positions& start_pos, const std::string& line, size_t& idx, Graph& graph)
{
    Positions current = start_pos;
    Positions result;

    while (idx < line.size() && line[idx] != ')' && line[idx] != '$') {
        Positions next;
        char c = line.at(idx++);
        switch(c) {
            case 'N': case 'S': case 'W': case 'E':
                for (auto p : current) {
                    auto q = p + DIRS_A.at(c);
                    graph[p].push_back(q);
                    next.insert(q);
                }
                break;
            case '(':
                next = _build_graph(current, line, idx, graph);
                assert(line[idx] == ')');
                ++idx;
                break;
            case '|':
                result.insert(current.cbegin(), current.cend());
                next = start_pos;
                break;
            default:
                throw std::runtime_error(std::format("invalid regex char '{}'", c));
        }
        current.swap(next);
    }
    result.insert(current.cbegin(), current.cend());
    return result;
}

Graph build_graph(const std::string& line) {
    assert(!line.empty() && line[0] == '^');

    Graph result;
    size_t idx = 1;
    Positions start{{0, 0}};

    auto pos = _build_graph(start, line, idx, result);
    assert(idx < line.size() && line[idx] == '$');
    // std::cout << "final positions: " << pos << std::endl;
    return result;
}

std::map<Point, int> bfs(const Graph& graph) {
    Point src(0, 0);
    std::map<Point, int> dists{{src, 0}};
    std::deque<Point> frontier{src};

    while (!frontier.empty()) {
        auto pos = frontier.front();
        frontier.pop_front();
        auto d = dists.at(pos);

        if (!graph.count(pos)) continue;
        for (auto new_pos : graph.at(pos)) {
            if (!dists.count(new_pos)) {
                dists[new_pos] = d + 1;
                frontier.push_back(new_pos);
            }
        }
    }
    return dists;
}


int solve_one(const std::string& line) {
    auto graph = build_graph(line);
    auto dists = bfs(graph);
    int result = -1;
    for (const auto& p : dists) {
        result = std::max(result, p.second);
    }
    return result;
}

int solve_two(const std::string& line) {
    auto graph = build_graph(line);
    auto dists = bfs(graph);
    int result = 0;
    for (const auto& p : dists) {
        if (p.second >= 1000) ++result;
    }
    return result;
}

int main() {
    assert(solve_one("^WNE$") == 3);
    assert(solve_one("^ENWWW(NEEE|SSE(EE|N))$") == 10);
    assert(solve_one("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$") == 18);

    std::string line;
    std::getline(std::cin, line);
    print_answer("one", solve_one(line));
    print_answer("two", solve_two(line));
}