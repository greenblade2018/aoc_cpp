#include "aoc.h"
using namespace aoc;

class ElfDevice {
public:
    using VecInt = std::vector<int>;
    using Program = std::vector<VecInt>;
    using Op = void(*)(const VecInt&, VecInt&);

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

    const static std::vector<Op> ops;
    const static std::map<std::string, int> name_index;

public:
    ElfDevice() : m_regs(6) {}
    void bind_ip(int n) { m_bind = n; }
    void run(const Program& program, int r0, int debug = 1);
    int& reg(int r) { return m_regs[r]; }

private:
    int m_ip = 0;
    int m_bind = -1;
    VecInt m_regs;
};

const std::vector<ElfDevice::Op> ElfDevice::ops {
    ElfDevice::addr, ElfDevice::addi,
    ElfDevice::mulr, ElfDevice::muli,
    ElfDevice::banr, ElfDevice::bani,
    ElfDevice::borr, ElfDevice::bori,
    ElfDevice::setr, ElfDevice::seti,
    ElfDevice::gtir, ElfDevice::gtri, ElfDevice::gtrr,
    ElfDevice::eqir, ElfDevice::eqri, ElfDevice::eqrr,
};
const std::map<std::string, int> ElfDevice::name_index {
    {"addr", 0}, {"addi", 1},
    {"mulr", 2}, {"muli", 3},
    {"banr", 4}, {"bani", 5},
    {"borr", 6}, {"bori", 7},
    {"setr", 8}, {"seti", 9},
    {"gtir", 10}, {"gtri", 11}, {"gtrr", 12},
    {"eqir", 13}, {"eqri", 13}, {"eqrr", 15},
};

void ElfDevice::run(const Program& program, int r0, int debug) {
    std::fill(m_regs.begin(), m_regs.end(), 0);
    m_regs[0] = r0;

    for (m_ip = 0; 0 <= m_ip && m_ip < (int)program.size(); ++m_ip) {
        m_regs[m_bind] = m_ip;
        if (debug >= 2) std::cout << "[ED] " << m_ip << ' ' << m_regs << ": " << program[m_ip];
        ops[program[m_ip][0]](program[m_ip], m_regs);
        if (debug >= 2) std::cout << " => " << m_regs << std::endl;
        m_ip = m_regs[m_bind];
    }
    if (debug >= 1) std::cout << "[ED] HALT ip = " << m_ip << std::endl;
}

void parse_input(int& bind, ElfDevice::Program& program) {
    std::string line;
    std::getline(std::cin, line);
    bind = line[4] - '0';

    while (std::getline(std::cin, line)) {
        std::vector<int> inst(4);
        inst[0] = ElfDevice::name_index.at(line.substr(0, 4));
        auto tmp = ints(line);
        for (size_t i = 0; i < 3; ++i) inst[i+1] = tmp[i];
        program.push_back(inst);
    }

    std::cout << "bind = " << bind << std::endl;
    // for (const auto& v : program) std::cout << v << std::endl;
    std::cout << "program size = " <<  program.size() << std::endl;
}

int solve_one(int bind, ElfDevice::Program program, int r0 = 0) {
    ElfDevice dev;
    dev.bind_ip(bind);
    dev.run(program, r0);
    return dev.reg(0);
}

int main() {
    int bind;
    ElfDevice::Program program;
    parse_input(bind, program);
    print_answer("one", solve_one(bind, program));
    print_answer("two", solve_one(bind, program, 1));
}