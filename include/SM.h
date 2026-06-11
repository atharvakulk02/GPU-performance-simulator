#pragma once
#include "Warp.h"
#include "Cache.h"
#include <vector>
#include <iostream>

struct SM {
    std::vector<Warp> warps;
    Cache l1;
    Cache l2;
    uint64_t cycles;
    int max_warps;
    int registers_per_thread;
    int shared_mem_per_block;
    int total_registers;
    int total_shared_mem;
    int threads_per_block;

SM(std::vector<Warp> warps,Cache l1, Cache l2, int max_warps, int registers_per_thread,int shared_mem_per_block,int total_registers,int total_shared_mem,int threads_per_block)
:
warps(warps),
l1(l1),
l2(l2),
cycles(0),
max_warps(max_warps),
registers_per_thread(registers_per_thread),
shared_mem_per_block(shared_mem_per_block),
total_registers(total_registers),
total_shared_mem(total_shared_mem),
threads_per_block(threads_per_block)
{}

void run(){
    while (!(all_finished())){
        cycles++;
        for(int i=0;i<compute_max_active_warps();i++){    
            warps[i].tick();
        }
        for(int i=0;i<compute_max_active_warps();i++){
            if(warps[i].is_ready()){
                warps[i].issue(l1,l2,cycles);
                break;
            }
            
        }
    }
}

bool all_finished(){
    for(auto& w : warps){
            if (!(w.is_finished())){
                return false;
            }
        }
    return true;
    }

void print_stats(){
    uint64_t sum=0;
    for(auto& w : warps){
        sum=sum+w.instructions_issued;
    }
    std::cout<<"Cycles : "<<cycles;
    std::cout<<"\nTotal instructions issued : "<<sum;
    std::cout<<"\nIPC : "<<double(sum)/cycles;
    std::cout<<"\nl1 hit rate : "<<double(l1.hits)/(l1.hits+l1.misses);
    std::cout<<"\nl2 hit rate : "<<double(l2.hits)/(l2.hits+l2.misses); 
    std::cout <<"\nMax active warps  : "<<compute_max_active_warps()<< "\n";
}

int compute_max_active_warps(){
    if (max_warps<(total_registers/(registers_per_thread*32))){
        if (max_warps<((total_shared_mem/shared_mem_per_block)*(threads_per_block/32))){
            return max_warps;
        }
        else{
            return ((total_shared_mem/shared_mem_per_block)*(threads_per_block/32));
        }
    }
    else{
        if ((total_registers/(registers_per_thread*32))<((total_shared_mem/shared_mem_per_block)*(threads_per_block/32))){
            return (total_registers/(registers_per_thread*32));
        }
        else{
            return ((total_shared_mem/shared_mem_per_block)*(threads_per_block/32));
        }
    }
}
};