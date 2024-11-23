#pragma once

#include <iostream>
#include <set>
#include <vector>

class Device {
public:
    using Code = std::vector<int>;
    using Program = std::vector<Code>;
    using Registers = std::vector<long>;

    static Device parse(std::istream& is);

public:
    Device(const Program& prog, int bind) : m_prog(prog), m_bind(bind), m_regs(6) {}
    void bind_ip(int n) { m_bind = n; }
    void reset() { m_ip = 0; m_regs.assign(6, 0); }
    void set_bp(int bp) { m_bps.insert(bp); }
    void clear_bp() { m_bps.clear(); }
    void run(int log = 1);
    long& reg(int r) { return m_regs[r]; }
    std::ostream& disasm(std::ostream& os);

private:
    bool operands_ready(const Code& code) const;
    bool is_goto(const Code& code) const { return code[3] == m_bind; }
    void guess_where(size_t ip, std::set<size_t>& where);
    void pass_goto(std::set<size_t>& entries, std::set<size_t>& exits);
    std::ostream& pass_print(std::ostream& os, const std::set<size_t>& entries);

private:
    Program m_prog;
    int m_bind = -1;
    int m_ip = 0;
    Registers m_regs;
    std::set<int> m_bps; // breakpoints
};
