# GPU Performance Simulator

A cycle-approximate GPU SM simulator built in C++ that models warp scheduling,
a two-level cache hierarchy with LRU eviction, and occupancy constraints.
Supports configurable instruction traces and scheduling policies, validated
against real hardware counters collected on an NVIDIA Tesla T4 via Nsight Compute.

## Architecture

The simulator is built around three core components:

**Warp** : a group of 32 threads that execute in lockstep. Each warp maintains
its own instruction trace, program counter, and stall state. When a warp issues
a memory instruction, it stalls until the cache hierarchy resolves the request.
While it waits, other warps can issue instructions, hiding the latency.

**Cache** : a two-level hierarchy (L1 and L2) with configurable sets,
associativity, and block size. Each cache uses LRU eviction to decide which
block to replace on a miss. Addresses are decoded into tag and set index using
bit manipulation. On a miss, the hierarchy falls back to a DRAM model with a
200-cycle penalty.

**SM (Streaming Multiprocessor)** : owns the warps and both caches, and runs
the cycle loop. Each cycle it ticks all active warps to decrement stall counters,
then selects the first ready warp to issue an instruction. The number of active
warps is capped by an occupancy model that accounts for register file pressure
and shared memory usage per thread block.

## Features

- Configurable warp count, instruction traces, and memory access patterns
- Two-level cache with LRU eviction and hit/miss tracking
- DRAM fallback with configurable latency
- Occupancy model based on register file and shared memory constraints
- Per-run stats: IPC, L1/L2 hit rates, active warp count, total cycles

## Building and Running

**Requirements:** g++ with C++17 support

```bash
g++ -std=c++17 -I include -o gpu_sim src/main.cpp
./gpu_sim
```

The default workload models a tiled matrix multiply (N=1024, TILE_SIZE=16)
across 16 warps with realistic sequential memory addresses.

## Validation: Tiled Matrix Multiply on NVIDIA Tesla T4

The simulator was validated by running an equivalent tiled matrix multiply
CUDA kernel on a Tesla T4 GPU and collecting hardware counters via Nsight Compute.

| Metric | Real T4 (Nsight Compute) | This Simulator |
|---|---|---|
| L1 hit rate | 6.13% | 6.59% |
| L2 hit rate | 80.49% | 3.14% |
| Active warps | 31.58 | 48 (occupancy limit) |

## Key Findings

**L1 hit rate matched closely (6.13% vs 6.59%).** The simulator accurately
captures the low L1 reuse behavior of tiled matrix multiply's global memory
accesses. Each tile element is loaded once from global memory, so L1 provides
little benefit at this level.

**L2 hit rate diverged significantly (80.49% vs 3.14%).** The real T4 achieves
high L2 utilization through hardware prefetching. The memory controller detects
sequential access patterns and pulls data into L2 ahead of time. The simulator
has no prefetcher, so most misses fall through to DRAM. This is the single
biggest gap between the model and real hardware.

**Active warp count differed (31.58 vs 48).** The T4 has 32 physical warp slots
per SM, while the simulator's occupancy model calculated 48 based on register
and shared memory constraints. Tuning those parameters to match T4 specs more
precisely would close this gap.

Overall the simulator provides a solid baseline for reasoning about warp
scheduling behavior and memory hierarchy performance, and correctly identifies
the memory-bound nature of the workload.
