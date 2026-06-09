#include "Warp.h"
#include "Cache.h"
#include "SM.h"
#include <iostream>

int main() {
    std::vector<Instruction> instrs = {
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::MEM, 0x1000},
        Instruction{InstrType::MEM, 0x1000},
        Instruction{InstrType::ALU, 0}
    };
    

    Cache l1(64, 4, 32);
    Cache l2(256, 8, 64);

    std::vector<Warp> warps = {
        Warp(0,instrs),
        Warp(1,instrs),
        Warp(2,instrs),
        Warp(3,instrs)
    };

    SM sm(warps,l1,l2);
    sm.run();
    sm.print_stats();
    return 0;
}
