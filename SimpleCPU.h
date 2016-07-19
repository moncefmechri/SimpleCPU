#pragma once

#include <array>
#include <iostream>
#include <libcachesim/CacheToolbox.h>

#include "CacheController.h"
#include "TraceReplayer.h"

class SimpleCPU
{
    TraceReplayer replayer_0;
    TraceReplayer replayer_1;
    unsigned instruction_latency;

    std::array<uint64_t, 2> instruction_count;
    std::array<uint64_t, 2> mem_accesses;
    std::array<uint64_t, 2> cycles;
    CacheController cache_controller;
public:
    SimpleCPU(const std::string& trace_file_0, const std::string& trace_file_1,
              const CacheConfig& L1_config, const CacheConfig& L2_config,
              unsigned instruction_latency, unsigned L1_latency, unsigned L2_latency,
              unsigned dram_latency);


    void run(void);
    void print_statistics(std::ostream& os = std::cout);
};
