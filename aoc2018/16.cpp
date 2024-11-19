#include "aoc.h"
using namespace aoc;

using Record = std::vector<std::vector<int>>;

void parse_input(std::vector<Record>& records, Record& program) {
    std::string line;
    bool first = true;
    while (first) {
        Record rec;
        for (int i = 0; i < 3; ++i) {
            std::getline(std::cin, line);
            if (line.empty()) {
                first = false;
                break;
            }
            rec.push_back(ints(line));
        }
        if (!first) break;
        records.push_back(rec);
        std::getline(std::cin, line);
    }

    std::cout << "records:\n"; for (int i = 0; i < 8; ++i) { std::cout << records[i] << std::endl; }

    std::getline(std::cin, line);
    while (std::getline(std::cin, line)) {
        program.push_back(ints(line));
    }

    std::cout << "program:\n"; for (int i = 0; i < 8; ++i) { std::cout << program[i] << std::endl; }
}

using VecInt = std::vector<int>;
using Op = void(*)(const VecInt&, VecInt&);

void addr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] + regs[code[2]]; }
void addi(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] + code[2]; }
void mulr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] * regs[code[2]]; }
void muli(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] * code[2]; }
void banr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] & regs[code[2]]; }
void bani(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] & code[2]; }
void borr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] | regs[code[2]]; }
void bori(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] | code[2]; }
void setr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]]; }
void seti(const VecInt& code, VecInt& regs) { regs[code[3]] = code[1]; }
void gtir(const VecInt& code, VecInt& regs) { regs[code[3]] = code[1] > regs[code[2]] ? 1 : 0; }
void gtri(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] > code[2] ? 1 : 0; }
void gtrr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] > regs[code[2]] ? 1 : 0; }
void eqir(const VecInt& code, VecInt& regs) { regs[code[3]] = code[1] == regs[code[2]] ? 1 : 0; }
void eqri(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] == code[2] ? 1 : 0; }
void eqrr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] == regs[code[2]] ? 1 : 0; }

std::vector<Op> ops {addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr};
std::vector<std::string> names {"addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"};

std::set<int> try_decode(const VecInt& before, const VecInt& code, const VecInt& after) {
    std::set<int> result;
    for (int i = 0; i < (int)ops.size(); ++i) {
        VecInt tmp(before);
        ops[i](code, tmp);
        // std::cout << i << ' ' << names[i] << ": " << tmp << std::endl;
        if (tmp == after) result.insert(i);
    }
    // std::cout << "try_decode:"; for (int n : result) {std::cout << ' ' << names[n];} std::cout << std::endl;
    return result;
}

VecInt decode_ops(const std::vector<Record>& records) {
    std::vector<std::set<int>> maybe(16);
    for (auto& s : maybe) {
        for (int i = 0; i < 16; ++i) s.insert(i);
    }

    for (const auto& rec : records) {
        auto s = try_decode(rec[0], rec[1], rec[2]);
        assert(!s.empty());
        int op = rec[1][0];
        std::set<int> new_s;
        std::set_intersection(s.cbegin(), s.cend(),
                              maybe[op].cbegin(), maybe[op].cend(),
                              std::inserter(new_s, new_s.begin()));
        assert(!new_s.empty());
        maybe[op] = new_s;
    }

    for (int k = 0; k < 1000; ++k) {
        for (size_t i = 0; i < maybe.size(); ++i) {
            assert(!maybe[i].empty());
            if (maybe[i].size() == 1) {
                int n = *maybe[i].begin();
                for (size_t j = 0; j < maybe.size(); ++j) {
                    if (i != j) maybe[j].erase(n);
                }
            }
        }
    }

    VecInt opcode(16);
    for (size_t i = 0; i < opcode.size(); ++i) {
        assert(maybe[i].size() == 1);
        opcode[i] = *(maybe[i].begin());
    }
    return opcode;
}

int solve_one(const std::vector<Record>& records) {
    int result = 0;
    for (const auto& rec : records) {
        if (try_decode(rec[0], rec[1], rec[2]).size() >= 3) ++result;
    }
    return result;
}

int solve_two(const std::vector<Record>& records, const Record& program) {
    auto opcode = decode_ops(records);
    std::cout << "opcode = " << opcode << std::endl;

    VecInt regs(4);
    for (const auto& inst : program) ops[opcode[inst[0]]](inst, regs);
    return regs[0];
}

int main() {
    std::cout << try_decode(VecInt{3, 2, 1, 1}, VecInt{9, 2, 1, 2}, VecInt{3, 2, 2, 1}) << std::endl;
    std::vector<Record> records;
    Record program;
    parse_input(records, program);
    print_answer("one", solve_one(records));
    print_answer("two", solve_two(records, program));
}