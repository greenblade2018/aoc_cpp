#include "aoc.h"

long checksum(const std::vector<int> blocks) {
    long result = 0;
    for (size_t i = 0; i < blocks.size(); ++i) {
        if (blocks[i] >= 0) result += (long)i * blocks[i];
    }
    return result;

}
long one(std::vector<int> blocks) {
    size_t n = blocks.size();
    for (size_t i = 0, j = n - 1; i < j; ++i, --j) {
        // i points to the first empty block; j points to the last file block.
        while (i < n && blocks[i] >= 0) ++i;
        while (j > 0 && blocks[j] < 0) --j;
        if (i >= j) break;
        blocks[i] = blocks[j];
        blocks[j] = -1;
    }
    // std::cout << "blocks = " << blocks << std::endl;

    return checksum(blocks);
}

int first_fit(const std::vector<std::pair<int, int>>& idle, int start, int length) {
    for (size_t i = 0; i < idle.size() && idle[i].first < start; ++i) {
        if (idle[i].second >= length) return i;
    }
    return -1;
}

long two(std::vector<int> blocks) {
    // idle node = (index, length)
    std::vector<std::pair<int, int>> busy, idle;
    int prev = 0;
    for (size_t i = 1; i < blocks.size(); ++i) {
        if (blocks[i] == blocks[i-1]) continue;
        int len = i - prev;
        if (blocks[prev] >= 0) busy.emplace_back(prev, len);
        else idle.emplace_back(prev, len);
        prev = i;
    }
    if (blocks.back() >= 0) busy.emplace_back(prev, blocks.size() - prev);
    else idle.emplace_back(prev, blocks.size() - prev);

    // std::cout << "busy = " << busy << std::endl;
    // std::cout << "idle = " << idle << std::endl;

    for (int i = busy.size() - 1; i >= 0; --i) {
        auto [src, length] = busy[i];
        int k = first_fit(idle, src, length);
        if (k >= 0) {
            int dst = idle[k].first;
            idle[k].first += length;
            idle[k].second -= length;
            assert(idle[k].second >= 0);

            for (int j = 0; j < length; ++j) {
                blocks[dst+j] = blocks[src+j];
                blocks[src+j] = -1;
            }
        }
    }
    // std::cout << "blocks = " << blocks << std::endl;

    return checksum(blocks);
}

int main() {
    std::string line;
    std::getline(std::cin, line);
    // std::cout << "line = " << line << std::endl;
    std::vector<int> blocks;
    for (size_t i = 0; i < line.size(); ++i) {
        int id = (i % 2 == 0) ? i / 2 : -1;
        for (char c = '0'; c < line[i]; ++c) blocks.push_back(id);
    }
    // std::cout << "blocks = " << blocks << std::endl;

    print_answer("one", one(blocks));
    print_answer("two", two(blocks));
}