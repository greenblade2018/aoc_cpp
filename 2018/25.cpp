#include "aoc.h"

using Point4D = std::vector<int>;
using Points = std::vector<Point4D>;
using Graph = std::map<int, std::vector<int>>;

int distance(const Point4D& a, const Point4D& b) {
    int result = 0;
    for (size_t i = 0; i < a.size(); ++i) result += std::abs(a[i] - b[i]);
    return result;
}

void dfs(int v, const Graph& graph, std::vector<bool>& visited) {
    assert(!visited[v]);
    visited[v] = true;
    for (int u : graph.at(v)) {
        if (!visited[u]) dfs(u, graph, visited);
    }
}

int component(const Graph& graph) {
    int result = 0;
    int n = graph.size();
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            ++result;
            dfs(i, graph, visited);
        }
    }
    return result;
}

int main() {
    auto points = read_input<Point4D>(std::cin, ints);
    Graph graph;
    // ensure every vertex is in graph.
    for (size_t i = 0; i < points.size(); ++i) graph[i] = std::vector<int>();

    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            if (distance(points[i], points[j]) <=3) {
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }
    std::cout << "graph: " << graph << std::endl;

    print_answer("one", component(graph));
}