#include "aoc.h"

struct Group {
    int id; // positive means immune system; negative means infections.
    long unit;
    long hp;
    long damage;
    std::string dtype;
    int init;
    std::set<std::string> weaks;
    std::set<std::string> immunes;

    long power() const { return unit * damage; }
    bool weak_to(const std::string& type) const { return weaks.count(type); }
    bool immune_to(const std::string& type) const { return immunes.count(type); }
};

using Groups = std::vector<Group>;

std::ostream& operator<<(std::ostream& os, const Group& g) {
    os << std::format("Group(id={}, unit={}, hp={}, damage={}, dtype={}, init={}", g.id, g.unit, g.hp, g.damage, g.dtype, g.init);
    os << ", weaks=" << g.weaks;
    os << ", immunes=" << g.immunes << ")";
    return os;
}

Group parse_group(int id, const std::string& line) {
    Group g;
    g.id = id;
    auto nums = ints(line);
    g.unit = nums.at(0);
    g.hp = nums.at(1);
    g.damage = nums.at(2);
    g.init = nums.at(3);

    static std::regex reg_dtype(R"((\w+) damage)");
    std::smatch mr;
    assert(std::regex_search(line, mr, reg_dtype));
    g.dtype = mr.str(1);

    static std::regex reg_weaks(R"(weak to ([a-z, ]+))");
    if (std::regex_search(line, mr, reg_weaks)) {
        for (const auto& w : words(mr.str(1))) g.weaks.insert(w);
    }

    static std::regex reg_immunes(R"(immune to ([a-z, ]+))");
    if (std::regex_search(line, mr, reg_immunes)) {
        for (const auto& w : words(mr.str(1))) g.immunes.insert(w);
    }

    return g;
}

void read_groups(Groups& groups) {
    std::string line;
    std::getline(std::cin, line);
    int co = 1;
    for (int id = 1; std::getline(std::cin, line); ++id) {
        if (line.empty()) {
            co = -co;
            id = 0;
            std::getline(std::cin, line);
            continue;
        }
        groups.push_back(parse_group(co * id, line));
    }
}

bool is_enemy(const Group& a, const Group& b) {
    return (a.id * b.id) < 0;
}

long calc_damage(const Group& attacker, const Group& defender) {
    assert(is_enemy(attacker, defender));
    if (defender.immune_to(attacker.dtype)) return 0;
    long dmg = attacker.power();
    if (defender.weak_to(attacker.dtype)) dmg *= 2;
    return dmg;
}

bool better_target(const Group& me, const Group& g1, const Group& g2) {
    assert(is_enemy(me, g1) && is_enemy(me, g2));
    auto dmg1 = calc_damage(me, g1), dmg2 = calc_damage(me, g2);
    if (dmg1 != dmg2) return dmg1 > dmg2;
    auto power1 = g1.power(), power2 = g2.power();
    if (power1 != power2) return power1 > power2;
    return g1.init > g2.init;
}

std::map<int, int> selection_phase(Groups& groups) {
    std::sort(groups.begin(), groups.end(), [](const Group& a, const Group& b) {
        auto pa = a.power(), pb = b.power();
        if (pa != pb) return pa > pb;
        return a.init > b.init;
    });
    // std::cout << "target selection order:";
    // for (const auto& g : groups) std::cout << " " << g.id;
    // std::cout << "\n";

    std::set<int> selected;
    std::map<int, int> targets;
    for (const auto& me : groups) {
        const Group* best = nullptr;
        for (const auto& enemy : groups) {
            if (!is_enemy(me, enemy) || selected.count(enemy.id)) continue;
            if (best == nullptr || better_target(me, enemy, *best)) best = &enemy;
        }
        if (best != nullptr && calc_damage(me, *best) > 0) {
            selected.insert(best->id);
            targets[me.id] = best->id;
        }
    }

    return targets;
}

void fight(Groups& groups) {
    auto targets = selection_phase(groups);
    // std::cout << "targets: " << targets << std::endl;

    std::sort(groups.begin(), groups.end(), [](const auto& g1, const auto& g2) { return g1.init > g2.init; });
    for (const auto& me : groups) {
        if (me.unit <= 0) {
            // std::cout << std::format("Group {} is DEAD!\n", me.id);
            continue;
        }
        if (!targets.count(me.id)) {
            // std::cout << std::format("Group {} has NO TARGET!\n", me.id);
            continue;
        }
        Group* enemy = nullptr;
        for (auto& g : groups) {
            if (g.id == targets[me.id]) {
                enemy = &g;
                break;
            }
        }
        assert(enemy != nullptr);

        long kill = calc_damage(me, *enemy) / enemy->hp;
        enemy->unit -= kill;
        // std::cout << std::format("Group {} attacks group {}, killing {} units.\n", me.id, enemy->id, kill);
    }
}

std::pair<long, long> count_units(const Groups& groups) {
    long immune = 0, infection = 0;
    for (const auto& g : groups) {
        if (g.id > 0) immune += g.unit;
        else infection += g.unit;
    }
    return {immune, infection};
}

std::pair<long, long> combat(Groups& groups, long boost = 0) {
    for (auto& g: groups) {
        if (g.id > 0) g.damage += boost;
    }

    // It's possible that no army can kill the other. We save the last surviving units as 'prev'.
    // If no units killed in this round, then it's a draw.
    std::pair<long, long> prev {0, 0};
    for (int round = 1; true; ++round) {
        // std::cout << std::format("===== ROUND {} =====\n", round);
        fight(groups);
        std::erase_if(groups, [](const auto& g) { return g.unit <= 0; });
        auto remain = count_units(groups);
        if (remain == prev) break;
        prev = remain;
        if (remain.first == 0 || remain.second == 0) break;
    }
    return prev;
}

long one(Groups groups) {
    auto [immune, infection] = combat(groups);
    return immune == 0 ? infection : immune;
}

long two(Groups groups) {
    long low = 0, high = 1000000;
    while (low + 1 < high) {
        long mid = (low + high) / 2;
        std::cout << std::format("try boost = {}.\n", mid);
        auto tmp = groups;
        auto [immune, infection] = combat(tmp, mid);
        if (infection == 0) {
            high = mid;
        } else {
            low = mid;
        }
    }

    return combat(groups, high).first;
}

int main() {
    // const std::string sample("18 units each with 729 hit points (weak to fire; immune to cold, slashing) with an attack that does 8 radiation damage at initiative 10");
    // std::cout << parse_group(0, sample) << std::endl;

    Groups groups;
    read_groups(groups);
    std::cout << "Groups:\n";
    for (const auto& g : groups) std::cout << g << std::endl;

    print_answer("one", one(groups));
    print_answer("two", two(groups));
}