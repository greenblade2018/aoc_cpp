#include "aoc.h"
using namespace aoc;

// Too lazy to free the memory.
struct Node {
    int nchild;
    int nmeta;
    std::vector<Node*> children;
    std::vector<int> meta;
};

Node *build_tree(const std::vector<int>& input, size_t& idx) {
    Node *node = new Node;
    node->nchild = input[idx++];
    node->nmeta = input[idx++];
    node->children.resize(node->nchild);
    node->meta.resize(node->nmeta);
    for (int i = 0; i < node->nchild; ++i) {
        node->children[i] = build_tree(input, idx);
    }
    for (int i = 0; i < node->nmeta; ++i) {
        node->meta[i] = input[idx++];
    }
    return node;
}

int sum_meta(const Node* node) {
    int result = std::accumulate(node->meta.cbegin(), node->meta.cend(), 0);
    for (const Node* ch : node->children) {
        result += sum_meta(ch);
    }
    return result;
}

int calc_value(const Node* node) {
    int result = 0;
    if (node->nchild == 0) {
        result = std::accumulate(node->meta.cbegin(), node->meta.cend(), 0);
    } else {
        for (int m : node->meta) {
            if (m < 1 || m > node->nchild) continue;
            --m;
            result += calc_value(node->children[m]);
        }
    }

    return result;
}

int main() {
    std::vector<int> inputs;
    std::copy(std::istream_iterator<int>(std::cin),
              std::istream_iterator<int>(),
              std::back_inserter(inputs));
    // std::cerr << inputs << std::endl;

    size_t idx = 0;
    auto root = build_tree(inputs, idx);
    assert(idx == inputs.size());

    print_answer("one", sum_meta(root));
    print_answer("two", calc_value(root));
}