#include "Warp.h"
#include "Cache.h"
#include "SM.h"
#include <iostream>

int main() {
    int N=1024;
    int TILE_SIZE=16;
    int num_warps=16;
    std::vector<Warp> warps;
    for (int i=0;i<num_warps;i++){
        std::vector<Instruction> instrs;
        for(int t=0;(t<N/TILE_SIZE);t++){
            instrs.push_back(Instruction{InstrType::MEM, (uint64_t)(i*TILE_SIZE*N*4)+(t*TILE_SIZE*4)});
            instrs.push_back(Instruction{InstrType::MEM, (uint64_t)(i*TILE_SIZE*4)+(t*TILE_SIZE*4*N)});
            for (int z=0;z<TILE_SIZE;z++){
                instrs.push_back(Instruction{InstrType::ALU, 0});
            }
        }
        warps.push_back(Warp(i,instrs));
    }

    Cache l1(64, 4, 32);
    Cache l2(256, 8, 64);

    SM sm(warps,l1,l2,64,32,8192,65536,49152,256);
    sm.run();
    sm.print_stats();
    return 0;
}
