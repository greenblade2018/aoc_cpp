#include "aoc.h"
#include "device.h"
using namespace aoc;

int main() {
    Device::parse(std::cin).disasm(std::cout);
}