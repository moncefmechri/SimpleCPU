#pragma once

#include <unordered_map>

#include "DualCoreExclusiveHierarchy.h"

#include <libcachesim/CacheToolbox.h>
#include <libcachesim/ExclusiveCache.h>

class CacheController
{
    DualCoreExclusiveHierarchy hierarchy;
    std::unordered_map<ExclusiveCache::ACCESS_STATUS, unsigned> latencies;
    CacheStats L1_stats[2];
    CacheStats L2_stats;

public:
    CacheController(const CacheConfig& L1_config, const CacheConfig& L2_config,
                    unsigned L1_latency, unsigned L2_latency, unsigned dram_latency);

    unsigned access(addr_t address, unsigned tid);
};
