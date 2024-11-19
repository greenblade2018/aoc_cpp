#include "aoc.h"
using namespace aoc;

using VecInt = std::vector<int>;
using VecString = std::vector<std::string>;
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
VecString names {"addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"};

VecInt try_decode(const VecInt& before, const VecInt& code, const VecInt& after) {
    VecInt result;
    for (int i = 0; i < (int)ops.size(); ++i) {
        VecInt tmp(before);
        ops[i](code, tmp);
        // std::cout << i << ' ' << names[i] << ": " << tmp << std::endl;
        if (tmp == after) result.push_back(i);
    }
    // std::cout << "try_decode:"; for (int n : result) {std::cout << ' ' << names[n];} std::cout << std::endl;
    return result;
}

using Record = std::vector<std::vector<int>>;
void parse_input(std::vector<Record>& records) {
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

    for (int i = 0; i < 8; ++i) { std::cout << records[i] << std::endl; }
}

int solve_one(std::vector<Record> records) {
    int result = 0;
    for (const auto& rec : records) {
        if (try_decode(rec[0], rec[1], rec[2]).size() >= 3) ++result;
    }
    return result;
}

int main() {
    std::cout << try_decode(VecInt{3, 2, 1, 1}, VecInt{9, 2, 1, 2}, VecInt{3, 2, 2, 1}) << std::endl;
    std::vector<Record> records;
    parse_input(records);
    print_answer("one", solve_one(records));
}