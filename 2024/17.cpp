#include "aoc.h"
using namespace std;

vector<long> regs{0, 0, 0};

long combo(int n) {
    return n <= 3 ? n : regs.at(n-4);
}

vector<int> run(const vector<int>& program) {
    vector<int> output;
    for (int ip = 0; ip >= 0 && ip < (int)program.size(); ip += 2) {
        int operand = program.at(ip+1);
        switch (program.at(ip)) {
            case 0: // adv
                regs[0] /= (1 << combo(operand));
                break;
            case 1: // bxl
                regs[1] ^= operand;
                break;
            case 2: // bst
                regs[1] = combo(operand) % 8;
                break;
            case 3: // jnz
                if (regs[0] != 0) ip = operand - 2;
                break;
            case 4: // bxc
                regs[1] ^= regs[2];
                break;
            case 5: // out
                output.push_back(combo(operand) % 8);
                break;
            case 6: // bdv
                regs[1] = regs[0] / (1 << combo(operand));
                break;
            case 7: // cdv
                regs[2] = regs[0] / (1 << combo(operand));
                break;
            default:
                cerr << ip << ": " << program[ip] << ' ' << program[ip+1] << endl;
                assert(false);
        }
    }
    return output;
}

void test_computer() {
    regs = vector<long>{0, 0, 9};
    run(vector<int>{2, 6});
    assert(regs[1] = 1);

    regs = {10, 0, 0};
    auto out = run({5,0,5,1,5,4});
    vector<int> expected{0, 1, 2};
    assert(out == expected);

    regs = {2024, 0, 0};
    out = run(vector<int>{0,1,5,4,3,0});
    expected = {4,2,5,6,7,7,7,7,3,1,0};
    assert(out == expected);
    assert(regs[0] == 0);

    regs = {0, 29, 0};
    run(vector<int>{1,7});
    assert(regs[1] == 26);

    regs = {0, 2024, 43690};
    run(vector<int>{4,0});
    assert(regs[1] == 44354);

    regs = {0, 0, 0};
}

string to_string(const vector<int>& v) {
    ostringstream oss;
    if (!v.empty()) {
        oss << v[0];
        for (size_t i = 1; i < v.size(); ++i) oss << ',' << v[i];
    }
    return oss.str();
}

int find_last(const vector<int>& program, int exp, long pre) {
    for (int a = 0; a < 8; ++a) {
        regs[0] = pre + a;
        auto out = run(program);
        if (out[0] == exp) return a;
    }
    return -1;
}

long find_a(const vector<int>& program, const vector<int>& expected, long pre) {
    for (size_t i = 0; i < expected.size(); ++i) {
        pre *= 8;
        auto a = find_last(program, expected[i], pre);
        if (a < 0) return -1;
        pre += a;
    }
    return pre;
}

long find_next(const vector<int>& program, const vector<int>& expected, size_t n, long pre) {
    cout << n << ' ' << expected[n] << ' ' << oct << pre << dec << endl;
    if (n == expected.size()) return pre;

    pre *= 8;
    for (int a = 0; a < 8; ++a, ++pre) {
        regs[0] = pre;
        auto out = run(program);
        if (out[0] == expected[n]) {
            long answer = find_next(program, expected, n+1, pre);
            if (answer >= 0) return answer;
        }
    }
    return -1;
}

long two(vector<int> program) {
    vector<int> expected(program.rbegin(), program.rend());
    cout << "expected: " << expected << endl;
    // get rid of the last 'jnz' to boost speed.
    program.pop_back();
    program.pop_back();

    return find_next(program, expected, 0, 0);
}

int main() {
    test_computer();

    string line;
    for (size_t i = 0; i < regs.size(); ++i) {
        getline(cin, line);
        regs[i] = longs(line)[0];
    }
    getline(cin, line);
    getline(cin, line);
    auto program = ints(line);
    cout << "regs = " << regs << ", program = " << program << endl;

    auto output = run(program);
    print_answer("one", to_string(output));
    print_answer("two", two(program));
}