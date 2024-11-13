#include "aoc.h"
using namespace aoc;

using Edge = std::pair<char, char>;
using Graph = std::map<char, std::string>;

Edge parse_line(const std::string& line) {
    char u = line[5], v = line[36];
    return Edge(u, v);
}

std::string topsort(const std::set<char>& nodes, const Graph& dag) {
    std::map<char, int> cnt;
    for (char c : nodes) cnt[c] = 0;
    for (const auto& [u, vs] : dag) {
        for (char v : vs) ++cnt[v];
    }

    std::string result;
    while (!cnt.empty()) {
        // find the first key with value of 0, since cnt is sorted by key.
        char u = '#';
        for (auto [k, v] : cnt) {
            if (v == 0) {
                u = k;
                break;
            }
        }

        assert(u != '#');
        result.push_back(u);
        cnt.erase(u);

        // remove outgoing edges from u
        if (auto it = dag.find(u); it != dag.cend()) {
            for (char v : it->second) assert(--cnt[v] >= 0);
        }
    }
    return result;
}

int topwork(const std::set<char>& nodes,
            const Graph& dag,
            int worker,
            int base_cost)
{
    std::map<char, int> cnt;
    for (char c : nodes) cnt[c] = 0;
    for (const auto& [u, vs] : dag) {
        for (char v : vs) ++cnt[v];
    }

    int time = 0;
    std::vector<std::pair<char, int>> work_list;
    while (!cnt.empty() || !work_list.empty()) {
        // process current work list
        for (auto& [job, cost] : work_list) {
            assert(--cost >= 0);
            if (cost == 0) {
                std::cerr << std::format("[{}] finish job {}.\n", time, job);
                // remove outgoing edges from u
                if (auto it = dag.find(job); it != dag.cend()) {
                    for (char v : it->second) assert(--cnt[v] >= 0);
                }
                ++worker;
            }
        }
        std::erase_if(work_list, [](const auto& p) {return p.second == 0;});

        while (worker > 0 && !cnt.empty()) {
            // find the first key with value of 0, since cnt is sorted by key.
            auto it = std::find_if(cnt.cbegin(),
                                   cnt.cend(),
                                   [](const auto& p) {return p.second == 0;});
            if (it == cnt.cend()) break;

            char job = it->first;
            int cost = base_cost + 1 + (job - 'A');
            std::cerr << std::format("[{}] Adding job {} with cost {}.\n", time, job, cost);
            work_list.emplace_back(job, cost);

            --worker;
            cnt.erase(it);
        }
        ++time;
    }
    return time - 1;
}

int main() {
    std::vector<Edge> edges = read_inputs<Edge>(std::cin, parse_line);

    std::set<char> nodes;
    for (auto [u, v] : edges) {
        nodes.insert(u);
        nodes.insert(v);
    }

    Graph dag;
    for (auto [u, v] : edges) dag[u].push_back(v);

    print_answer("one", topsort(nodes, dag));
    print_answer("two", topwork(nodes, dag, 5, 60));
}