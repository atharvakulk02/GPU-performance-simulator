#include "Warp.h"
#include "Cache.h"
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

    Warp w(0, instrs);

    while (!w.is_finished()) {
        w.tick();
        if (w.is_ready()) {
            w.issue(l1,l2);
        }
    }

    std::cout << "Instructions issued: " << w.instructions_issued << "\n";
    std::cout << "Cycles ready      : " << w.cycles_ready << "\n";
    std::cout << "Cycles stalled    : " << w.cycles_stalled << "\n";

    return 0;
}
