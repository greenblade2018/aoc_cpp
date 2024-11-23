#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "device.h"
#include "aoc.h"

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

static const std::vector<std::string> op_symbols {
    "+", "+", "*", "*", "&", "&", "|", "|", "#", "#", ">", ">", ">", "==", "==", "==",
};

static void execute(const Device::Code& code, Device::Registers& regs) {
    switch (code[0]) {
        case 0: // addr
            regs[code[3]] = regs[code[1]] + regs[code[2]];
            break;
        case 1: // addi
            regs[code[3]] = regs[code[1]] + code[2];
            break;
        case 2: // mulr
            regs[code[3]] = regs[code[1]] * regs[code[2]];
            break;
        case 3: // muli
            regs[code[3]] = regs[code[1]] * code[2];
            break;
        case 4: // banr
            regs[code[3]] = regs[code[1]] & regs[code[2]];
            break;
        case 5: // bani
            regs[code[3]] = regs[code[1]] & code[2];
            break;
        case 6: // borr
            regs[code[3]] = regs[code[1]] | regs[code[2]];
            break;
        case 7: // bori
            regs[code[3]] = regs[code[1]] | code[2];
            break;
        case 8: // setr
            regs[code[3]] = regs[code[1]];
            break;
        case 9: // seti
            regs[code[3]] = code[1];
            break;
        case 10: // gtir
            regs[code[3]] = code[1] > regs[code[2]] ? 1 : 0;
            break;
        case 11: // gtri
            regs[code[3]] = regs[code[1]] > code[2] ? 1 : 0;
            break;
        case 12: // gtrr
            regs[code[3]] = regs[code[1]] > regs[code[2]] ? 1 : 0;
            break;
        case 13: // eqir
            regs[code[3]] = code[1] == regs[code[2]] ? 1 : 0;
            break;
        case 14: // eqri
            regs[code[3]] = regs[code[1]] == code[2] ? 1 : 0;
            break;
        case 15: // eqrr
            regs[code[3]] = regs[code[1]] == regs[code[2]] ? 1 : 0;
            break;
        default:
            throw std::runtime_error(std::format("invalid opcode: {}", code[0]));
    }
}
static std::string to_string(size_t ip, const Device::Code& code) {
    return std::format("{}: {} {} {} {}", ip, index_name[code[0]], code[1], code[2], code[3]);
}

Device Device::parse(std::istream& is) {
    std::string line;
    std::getline(std::cin, line);
    int bind = line[4] - '0';

    Device::Program prog;
    while (std::getline(std::cin, line)) {
        Code code(4);
        code[0] = name_index.at(line.substr(0, 4));
        auto tmp = aoc::ints(line);
        for (size_t i = 0; i < 3; ++i) code[i+1] = tmp[i];
        prog.push_back(code);
    }

    std::cout << std::format("bind = {}, size = {}\n", bind, prog.size());
    return Device(prog, bind);
}

//
// Device implementations
//

void Device::run(int log) {
    while (0 <= m_ip && m_ip < (int)m_prog.size()) {
        m_regs[m_bind] = m_ip;
        if (log >= 2) std::cout << "[D] " << m_regs << " | " << to_string(m_ip, m_prog[m_ip]);
        execute(m_prog[m_ip], m_regs);
        if (log >= 2) std::cout << " => " << m_regs << std::endl;
        m_ip = m_regs[m_bind];
        ++m_ip;
        // must break after executing current code, otherwise it won't continue next time.
        if (m_bps.count(m_ip)) return;
    }
    if (log >= 1) std::cout << "[D] HALT ip = " << m_ip << std::endl;
}

std::ostream& Device::disasm(std::ostream& os) {
    std::cout << "goto pass ...\n";
    std::set<size_t> entries, exits;
    pass_goto(entries, exits);
    std::cout << "entries: " << entries << '\n';
    std::cout << "exits  : " << exits << '\n';

    std::cout << "print pass ...\n";
    return pass_print(os, entries);
}

// private methods

static bool use_reg(const Device::Code& code, int idx) {
    auto op = code[0];
    switch (idx) {
        case 1:
            return op <= 8 || op == 11 || op == 12 || op == 14 || op == 15;
        case 2:
            return (op <= 6 && op % 2 == 0) || op == 10 || op == 12 || op == 13 || op == 15;
        case 3:
            return true;
        default:
            throw std::out_of_range(std::format("invalide code index: {}", idx));
    }
    // unreachable
    return false;
}

bool Device::operands_ready(const Code& code) const {
    for (int i = 1; i <= 2; ++i) {
        if (use_reg(code, i) && m_regs[code[i]] < 0) return false;
    }
    return true;
}

void Device::guess_where(size_t ip, std::set<size_t>& where) {
    const Code& code = m_prog[ip];

    // Case #1: current code is 'addr x <ip> <ip>' or 'addr <ip> x <ip>'
    if (code[0] == name_index.at("addr") && (code[1] == m_bind || code[2] == m_bind)) {
        // If previous code is compare, then goto ip+1 or ip+2.
        if (ip > 0 && m_prog[ip-1][0] >= name_index.at("gtir")) {
            where.insert(ip + 1);
            where.insert(ip + 2);
            return;
        }
        // If next code is goto, then goto ip+1 or ip+2.
        if (ip + 1 < m_prog.size() && is_goto(m_prog[ip+1])) {
            where.insert(ip + 1);
            where.insert(ip + 2);
            return;
        }
    }
}

void Device::pass_goto(std::set<size_t>& entries, std::set<size_t>& exits) {
    entries.insert(0);

    for (size_t ip = 0; ip < m_prog.size(); ++ip) {
        const Code& code = m_prog[ip];
        if (!is_goto(code)) continue;
        exits.insert(ip);

        std::fill(m_regs.begin(), m_regs.end(), -1);
        m_regs[m_bind] = ip;
        if (operands_ready(code)) {
            execute(code, m_regs);
            entries.insert(m_regs[m_bind] + 1);
        } else {
            guess_where(ip, entries);
        }
    }
}

std::ostream& Device::pass_print(std::ostream& os, const std::set<size_t>& entries) {
    os << "#ip " << m_bind << '\n';

    size_t ipw = std::to_string(m_prog.size()).size();
    std::fill(m_regs.begin(), m_regs.end(), -1);
    for (size_t ip = 0; ip < m_prog.size(); ++ip) {
        os << std::format("{:>{}}: ", ip, ipw);

        if (entries.count(ip)) std::fill(m_regs.begin(), m_regs.end(), -1);
        m_regs[m_bind] = ip;
        const Code& code = m_prog[ip];
        if (operands_ready(code)) {
            execute(code, m_regs);
        } else {
            m_regs[code[3]] = -1;
        }

        std::string sa, sb, sc;
        sa = use_reg(code, 1) ? std::format("r{}", code[1]) : std::format("{}", code[1]);
        sb = use_reg(code, 2) ? std::format("r{}", code[2]) : std::format("{}", code[2]);
        sc = std::format("r{}", code[3]);

        if (is_goto(m_prog[ip])) {
            os << "goto ";
            if (m_regs[m_bind] >= 0) {
                os << (m_regs[m_bind] + 1);
            } else {
                os << std::format("{} {} {}  ", sa, op_symbols[code[0]], sb);
                std::set<size_t> where;
                guess_where(ip, where);
                os << where;
            }
        } else if (code[0] == 8 || code[0] == 9) {    // setr seti
            os << std::format("{} = {};", sc, sa);
        } else {
            os << std::format("{} = {} {} {};", sc, sa, op_symbols[code[0]], sb);
        }

        if (m_regs[code[3]] >= 0 && !is_goto(code)) os << "\t# " << m_regs[code[3]];
        os << '\n';
    }

    os.flush();
    return os;
}

// End of Device