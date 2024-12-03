#include "aoc.h"


std::vector<int> recipes = {3, 7};

void generate_recipes(size_t n) {
    size_t e1 = 0, e2 = 1;
    while (recipes.size() < n) {
        int k = recipes[e1] + recipes[e2];
        // Careful: n could be 0, in which case we should add one 0 to recipes.
        if (k >= 10) recipes.push_back(k / 10);
        recipes.push_back(k % 10);

        e1 = (e1 + 1 + recipes[e1]) % recipes.size();
        e2 = (e2 + 1 + recipes[e2]) % recipes.size();
    }
}

char itoc(int n) {
    return '0' + (char)n;
}

int main() {
    size_t input = 360781;
    generate_recipes(30'000'000UL);
    std::string rss;
    std::transform(&recipes[input], &recipes[input + 10], std::back_inserter(rss), itoc);
    print_answer("one", rss);

    rss.clear();
    std::transform(recipes.cbegin(), recipes.cend(), std::back_inserter(rss), itoc);
    print_answer("two", rss.find(std::to_string(input)));
}