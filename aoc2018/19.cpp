#include "aoc.h"
using namespace aoc;

using VecInt = std::vector<int>;
using Program = std::vector<VecInt>;
using Op = void(*)(const VecInt&, VecInt&);

static void addr(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0 && regs[code[2]] >= 0) ?
                    regs[code[1]] + regs[code[2]] :
                    -1;
}

static void addi(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0) ?
                    regs[code[1]] + code[2] :
                    -1;
}

static void mulr(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0 && regs[code[2]] >= 0) ?
                    regs[code[1]] * regs[code[2]] :
                    -1;
}

static void muli(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0) ?
                    regs[code[1]] * code[2] :
                    -1;
}

static void banr(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0 && regs[code[2]] >= 0) ?
                    regs[code[1]] & regs[code[2]] :
                    -1;
}

static void bani(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0) ?
                    regs[code[1]] & code[2] :
                    -1;
}

static void borr(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0 && regs[code[2]] >= 0) ?
                    regs[code[1]] | regs[code[2]] :
                    -1;
}

static void bori(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0) ?
                    regs[code[1]] | code[2] :
                    -1;
}

static void setr(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0) ? regs[code[1]] : -1;
}

static void seti(const VecInt& code, VecInt& regs) {
    regs[code[3]] = code[1];
}

static void gtir(const VecInt& code, VecInt& regs) {
    regs[code[3]] = regs[code[2]] >= 0 ?
                    (code[1] > regs[code[2]] ? 1 : 0) :
                    -1;
} 

static void gtri(const VecInt& code, VecInt& regs) {
    regs[code[3]] = regs[code[1]] >= 0 ?
                    (regs[code[1]] > code[2] ? 1 : 0) :
                    -1;
}

static void gtrr(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0 && regs[code[2]] >= 0) ?
                    (regs[code[1]] > regs[code[2]] ? 1 : 0) :
                    -1;
}

static void eqir(const VecInt& code, VecInt& regs) {
    regs[code[3]] = regs[code[2]] >= 0 ?
                    (code[1] == regs[code[2]] ? 1 : 0) :
                    -1;
}

static void eqri(const VecInt& code, VecInt& regs) {
    regs[code[3]] = regs[code[1]] >= 0 ?
                    (regs[code[1]] == code[2] ? 1 : 0) :
                    -1;
}

static void eqrr(const VecInt& code, VecInt& regs) {
    regs[code[3]] = (regs[code[1]] >= 0 && regs[code[2]] >= 0) ?
                    (regs[code[1]] == regs[code[2]] ? 1 : 0) :
                    -1;
}

static const std::vector<Op> ops {
    addr, addi,
    mulr, muli,
    banr, bani,
    borr, bori,
    setr, seti,
    gtir, gtri, gtrr,
    eqir, eqri, eqrr,
};

static const std::vector<std::string> op_symbols {
    "+", "+", "*", "*", "&", "&", "|", "|", "#", "#", ">", ">", ">", "==", "==", "==",
};

static const std::map<std::string, int> name_index {
    {"addr", 0}, {"addi", 1},
    {"mulr", 2}, {"muli", 3},
    {"banr", 4}, {"bani", 5},
    {"borr", 6}, {"bori", 7},
    {"setr", 8}, {"seti", 9},
    {"gtir", 10}, {"gtri", 11}, {"gtrr", 12},
    {"eqir", 13}, {"eqri", 14}, {"eqrr", 15},
};

static const std::vector<std::string> index_name {
    "addr", "addi",
    "mulr", "muli",
    "banr", "bani",
    "borr", "bori",
    "setr", "seti",
    "gtir", "gtri", "gtrr",
    "eqir", "eqri", "eqrr",
};

static std::string to_string(size_t ip, const VecInt& code) {
    return std::format("{}: {} {} {} {}", ip, index_name[code[0]], code[1], code[2], code[3]);
}

class ElfDevice {
public:
    ElfDevice() : m_regs(6) {}
    void bind_ip(int n) { m_bind = n; }
    void run(const Program& program, int r0, int debug = 1);
    int& reg(int r) { return m_regs[r]; }
    std::ostream& disasm(std::ostream& os, const Program& program);

private:
    bool is_goto(const VecInt& code) { return code[3] == m_bind; }
    void guess_where(const Program& program, size_t ip, std::set<size_t>& where);
    void pass_goto(const Program& program, std::set<size_t>& entries, std::set<size_t>& exits);
    std::ostream& pass_print(std::ostream& os, const Program& program, const std::set<size_t>& entries);

private:
    int m_ip = 0;
    int m_bind = -1;
    VecInt m_regs;
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

std::ostream& ElfDevice::disasm(std::ostream& os, const Program& program) {
    std::cout << "goto pass ...\n";
    std::set<size_t> entries, exits;
    pass_goto(program, entries, exits);
    std::cout << "entries: " << entries << '\n';
    std::cout << "exits  : " << exits << '\n';

    std::cout << "print pass ...\n";
    return pass_print(os, program, entries);
}

void ElfDevice::guess_where(const Program& program, size_t ip, std::set<size_t>& where) {
    const VecInt& code = program[ip];

    // Case #1: current code is 'addr x <ip> <ip>' or 'addr <ip> x <ip>'
    if (code[0] == name_index.at("addr") && (code[1] == m_bind || code[2] == m_bind)) {
        // If previous code is compare, then goto ip+1 or ip+2.
        if (ip > 0 && program[ip-1][0] >= name_index.at("gtir")) {
            where.insert(ip + 1);
            where.insert(ip + 2);
            return;
        }
        // If next code is goto, then goto ip+1 or ip+2.
        if (ip + 1 < program.size() && is_goto(program[ip+1])) {
            where.insert(ip + 1);
            where.insert(ip + 2);
            return;
        }
    }
}

void ElfDevice::pass_goto(const Program& program, std::set<size_t>& entries, std::set<size_t>& exits) {
    entries.insert(0);

    for (size_t ip = 0; ip < program.size(); ++ip) {
        const VecInt& code = program[ip];
        if (!is_goto(code)) continue;
        exits.insert(ip);

        std::fill(m_regs.begin(), m_regs.end(), -1);
        m_regs[m_bind] = ip;
        ops[code[0]](code, m_regs);
        if (m_regs[m_bind] >= 0) {
            entries.insert(m_regs[m_bind] + 1);
        } else {
            guess_where(program, ip, entries);
        }
    }
}

std::ostream& ElfDevice::pass_print(std::ostream& os, const Program& program, const std::set<size_t>& entries) {
    os << "#ip " << m_bind << '\n';

    size_t ipw = std::to_string(program.size()).size();
    std::fill(m_regs.begin(), m_regs.end(), -1);
    for (size_t ip = 0; ip < program.size(); ++ip) {
        os << std::format("{:>{}}: ", ip, ipw);

        int op = program[ip][0], a = program[ip][1], b = program[ip][2], c = program[ip][3];
        if (entries.count(ip)) std::fill(m_regs.begin(), m_regs.end(), -1);
        m_regs[m_bind] = ip;
        ops[op](program[ip], m_regs);

        std::string sa, sb, sc;
        sa = (op <= 8 || op == 11 || op == 12 || op == 14 || op == 15) ?
             std::format("r{}", a) :
             std::format("{}", a);
        sb = (op == 0 || op == 2 || op == 4 || op == 6 || op == 10 || op == 12 || op == 13 || op == 15) ?
             std::format("r{}", b) :
             std::format("{}", b);
        sc = std::format("r{}", c);

        if (is_goto(program[ip])) {
            os << "goto ";
            if (m_regs[m_bind] >= 0) {
                os << (m_regs[m_bind] + 1);
            } else {
                os << std::format("{} {} {}  ", sa, op_symbols[op], sb);
                std::set<size_t> where;
                guess_where(program, ip, where);
                os << where;
            }
        } else if (op == 8 || op == 9) {    // setr seti
            os << std::format("{} = {};", sc, sa);
        } else {
            os << std::format("{} = {} {} {};", sc, sa, op_symbols[op], sb);
        }

        if (m_regs[c] >= 0) os << "\t# " << m_regs[c];
        os << '\n';
    }

    os.flush();
    return os;
}

// End of ElfDevice

void parse_input(int& bind, Program& program) {
    std::string line;
    std::getline(std::cin, line);
    bind = line[4] - '0';

    while (std::getline(std::cin, line)) {
        std::vector<int> inst(4);
        inst[0] = name_index.at(line.substr(0, 4));
        auto tmp = ints(line);
        for (size_t i = 0; i < 3; ++i) inst[i+1] = tmp[i];
        program.push_back(inst);
    }

    std::cout << "bind = " << bind << std::endl;
    // for (const auto& v : program) std::cout << v << std::endl;
    std::cout << "program size = " <<  program.size() << std::endl;
}

int solve_one(int bind, const Program& program, int r0 = 0) {
    ElfDevice dev;
    dev.bind_ip(bind);
    dev.run(program, r0);
    return dev.reg(0);
}

int factor_sum(int n) {
    int result = 0;
    for (int i = 1; i <= n; ++i) {
        if (n % i == 0) result += i;
    }
    return result;
}

int main() {
    int bind;
    Program program;
    parse_input(bind, program);
    // print_answer("one", factor_sum(947));
    // print_answer("two", factor_sum(947 + 10550400));
    ElfDevice dev;
    dev.bind_ip(bind);
    dev.disasm(std::cout, program);
    std::cout << std::endl;
}