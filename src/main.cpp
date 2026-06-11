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
    std::vector<Instruction> instrs2 = {
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::MEM, 0x2000},
        Instruction{InstrType::MEM, 0x2000},
        Instruction{InstrType::ALU, 0}
    };
    std::vector<Instruction> instrs3 = {
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::MEM, 0x3000},
        Instruction{InstrType::MEM, 0x3000},
        Instruction{InstrType::ALU, 0}
    };
    std::vector<Instruction> instrs4 = {
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::ALU, 0},
        Instruction{InstrType::MEM, 0x4000},
        Instruction{InstrType::MEM, 0x4000},
        Instruction{InstrType::ALU, 0}
    };
    

    Cache l1(64, 4, 32);
    Cache l2(256, 8, 64);

    std::vector<Warp> warps = {
        Warp(0,instrs),
        Warp(1,instrs2),
        Warp(2,instrs3),
        Warp(3,instrs4)
    };

    SM sm(warps,l1,l2,64,32,8192,65536,49152,256);
    sm.run();
    sm.print_stats();
    return 0;
}
