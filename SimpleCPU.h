#pragma once

#include <iostream>
#include <libcachesim/CacheToolbox.h>

#include "CacheController.h"
#include "TraceReplayer.h"

class SimpleCPU
{
    TraceReplayer replayer_0;
    TraceReplayer replayer_1;
    unsigned instruction_latency;
    uint64_t instruction_count[2];
    uint64_t mem_accesses[2];
    uint64_t cycles[2];

    CacheController cache_controller;
public:
    SimpleCPU(const std::string& filename_0, const std::string& filename_1,
              const CacheConfig& L1_config, const CacheConfig& L2_config,
              unsigned instruction_latency, unsigned L1_latency, unsigned L2_latency,
              unsigned dram_latency);


    void run(void);
    void print_statistics(std::ostream& os = std::cout);
};
