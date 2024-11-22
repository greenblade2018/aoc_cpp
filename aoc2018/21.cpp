#include <iostream>
using namespace std;

int main() {
    long long r4 = 0x10000;
    long long r1 = 12772194;
    long long r3 = r4 & 0xFF;
    r1 = (r1 + r3) & 0xFFFFFF;
    r1 = (r1 * 65899) & 0xFFFFFF;

    while (r4 > 256) {
        r4 = r4 / 256;
        r3 = r4 & 0xFF;
        r1 = (r1 + r3) & 0xFFFFFF;
        r1 = (r1 * 65899) & 0xFFFFFF;

    }

    cout << r1 << endl;
}

/*



*/