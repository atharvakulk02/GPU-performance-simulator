#pragma once
#include <vector>
#include <cstdint>

struct Cache {
    int num_sets;
    int associativity;
    int block_size;
    uint64_t hits;
    uint64_t misses;
    std::vector<std::vector<uint64_t>> tags;
    std::vector<std::vector<bool>> valid;
    std::vector<std::vector<uint64_t>> lru;

    Cache(int num_sets, int associativity, int block_size)
        : num_sets(num_sets),
          associativity(associativity),
          block_size(block_size),
          tags(num_sets, std::vector<uint64_t>(associativity, 0)),
          valid(num_sets, std::vector<bool>(associativity, false)),
          hits(0),
          misses(0),
          lru(num_sets, std::vector<uint64_t>(associativity,0))
    {}

    int ilog2(int n) const {
        int i = 0;
        while (1 << i != n)
            i++;
        return i;
    }

    void decode(uint64_t address, uint64_t &tag, int &index) const {
        int offset_bits = ilog2(block_size);
        int index_bits  = ilog2(num_sets);
        index = (address >> offset_bits) & ((1 << index_bits) - 1);
        tag   = address >> (offset_bits + index_bits);
    }

    bool access(uint64_t address, uint64_t time) {
        uint64_t tag;
        int index;
        decode(address, tag, index);
        for (int way = 0; way < associativity; way++) {
            if (valid[index][way] && tags[index][way] == tag) {
                hits++;
                lru[index][way]=time;
                return true;
            }
        }
        misses++;
        int lru_way=0;
        for (int way=1;way<associativity;way++){
            if(lru[index][way]<lru[index][lru_way]){
               lru_way=way; 
            }
        }
        tags[index][lru_way]=tag;
        valid[index][lru_way]=true;
        lru[index][lru_way]=time;
        return false;
    }
};
