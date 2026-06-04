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

    Cache(int num_sets, int associativity, int block_size)
        : num_sets(num_sets),
          associativity(associativity),
          block_size(block_size),
          tags(num_sets, std::vector<uint64_t>(associativity, 0)),
          valid(num_sets, std::vector<bool>(associativity, false)),
          hits(0),
          misses(0)
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

    bool access(uint64_t address) {
        uint64_t tag;
        int index;
        decode(address, tag, index);
        for (int way = 0; way < associativity; way++) {
            if (valid[index][way] && tags[index][way] == tag) {
                hits++;
                return true;
            }
        }
        misses++;
        tags[index][0] = tag;
        valid[index][0] = true;
        return false;
    }
};
