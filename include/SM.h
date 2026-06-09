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

SM(std::vector<Warp> warps,Cache l1, Cache l2)
:
warps(warps),
l1(l1),
l2(l2),
cycles(0)
{}

void run(){
    while (!(all_finished())){
        cycles++;
        for(auto& w : warps){    
            w.tick();
        }
        for(auto& w : warps){
            if(w.is_ready()){
                w.issue(l1,l2);
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
    std::cout<<"\nl2 hit rate : "<<double(l2.hits)/(l2.hits+l2.misses)<<"\n"; 
}
};