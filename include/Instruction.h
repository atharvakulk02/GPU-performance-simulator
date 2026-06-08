#pragma once
#include <cstdint>

enum class InstrType { ALU, MEM };

struct Instruction {
    InstrType type;
    uint64_t address;
};
