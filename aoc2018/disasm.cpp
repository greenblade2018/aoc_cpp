#include "aoc.h"
#include "device.h"


int main() {
    Device::parse(std::cin).disasm(std::cout);
}