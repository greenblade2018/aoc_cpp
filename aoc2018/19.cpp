#include "aoc.h"
using namespace aoc;

class ElfDevice {
public:
    using VecInt = std::vector<int>;
    using Op = void(*)(const VecInt&, VecInt&);
    using Program = std::vector<std::string>;

    static void addr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] + regs[code[2]]; }
    static void addi(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] + code[2]; }
    static void mulr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] * regs[code[2]]; }
    static void muli(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] * code[2]; }
    static void banr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] & regs[code[2]]; }
    static void bani(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] & code[2]; }
    static void borr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] | regs[code[2]]; }
    static void bori(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] | code[2]; }
    static void setr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]]; }
    static void seti(const VecInt& code, VecInt& regs) { regs[code[3]] = code[1]; }
    static void gtir(const VecInt& code, VecInt& regs) { regs[code[3]] = code[1] > regs[code[2]] ? 1 : 0; }
    static void gtri(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] > code[2] ? 1 : 0; }
    static void gtrr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] > regs[code[2]] ? 1 : 0; }
    static void eqir(const VecInt& code, VecInt& regs) { regs[code[3]] = code[1] == regs[code[2]] ? 1 : 0; }
    static void eqri(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] == code[2] ? 1 : 0; }
    static void eqrr(const VecInt& code, VecInt& regs) { regs[code[3]] = regs[code[1]] == regs[code[2]] ? 1 : 0; }

    static std::map<std::string, Op> ops;

public:
    ElfDevice() : m_regs(6) {}
    void run(const Program& program);

private:
    void instruct(const std::string& inst);

private:
    int m_ip = 0;
    int m_bind = -1;
    VecInt m_regs;
};

std::map<std::string, ElfDevice::Op> ElfDevice::ops {
    {"addr", addr}, {"addi", addi},
    {"mulr", mulr}, {"muli", muli},
    {"banr", banr}, {"bani", bani},
    {"borr", borr}, {"bori", bori},
    {"setr", setr}, {"seti", seti},
    {"gtir", gtir}, {"gtri", gtri}, {"gtrr", gtrr},
    {"eqir", eqir}, {"eqri", eqri}, {"eqrr", eqrr},
};

static int as_digit(char c) { return c - '0'; }

void ElfDevice::run(const Program& program) {
    std::fill(m_regs.begin(), m_regs.end(), 0);
    m_bind = -1;
    for (m_ip = 0; )
        if (line[0] == '#') {
            m_bind = as_digit(line[4]);
        } else {
            VecInt code(4);
            VecInt tmp = ints(line);
            for (int i = 0; i < 3; ++i) code[i+1] = tmp[i];
            auto opstr = line.substr(0, 4);
            ops[opstr](code, m_regs);
        }
    }
}