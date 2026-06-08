#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Instruction.h"
#include "Cache.h"

enum class WarpState { READY, MEM_STALL, EXEC_STALL, FINISHED };

struct Warp {
    int id;
    WarpState state;
    int stall_cycles_remaining;
    std::vector<Instruction> instructions;
    int pc;
    uint64_t cycles_ready;
    uint64_t cycles_stalled;
    uint64_t instructions_issued;

    Warp(int id, std::vector<Instruction> instructions)
        : id(id),
          instructions(instructions),
          state(WarpState::READY),
          stall_cycles_remaining(0),
          pc(0),
          cycles_ready(0),
          cycles_stalled(0),
          instructions_issued(0)
    {}

    bool is_ready() const { return (state == WarpState::READY && pc < (int)instructions.size()); }
    bool is_finished() const { return (pc >= (int)instructions.size()); }

    void tick() {
        if (state == WarpState::MEM_STALL || state == WarpState::EXEC_STALL) {
            stall_cycles_remaining--;
            cycles_stalled++;
            if (stall_cycles_remaining == 0) {
                state = WarpState::READY;
            }
            if (is_finished()) {
                state = WarpState::FINISHED;
            }
        }
        if (state == WarpState::READY && !is_finished()) {
            cycles_ready++;
        }
    }

    int issue(Cache& l1, Cache& l2) {
        int latency = 1;
        Instruction instr = instructions[pc];
        pc++;
        instructions_issued++;
        if (instr.type == InstrType::MEM) {
            if (l1.access(instr.address)){
                latency=4;
            }
            else{
                if(l2.access(instr.address)){
                    latency=30;
                }
                else{
                    latency=200;
                }
            }
            state = WarpState::MEM_STALL;
            stall_cycles_remaining = latency;
        }
        return latency;
    }
};
