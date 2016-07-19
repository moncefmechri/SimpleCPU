#pragma once

#include <array>
#include <unordered_map>

#include "DualCoreExclusiveHierarchy.h"

#include <libcachesim/CacheToolbox.h>
#include <libcachesim/ExclusiveCache.h>

class CacheController
{
    DualCoreExclusiveHierarchy hierarchy;

    /* int is used as the key type instead of ACCESS_STATUS because
     * gcc <= 4.9.3 does not support enum class types as key. See
     * https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key */
    std::unordered_map<int, unsigned> latencies;
    std::array<CacheStats, 2> L1_stats;
    std::array<CacheStats, 2> L2_stats;

public:
    CacheController(const CacheConfig& L1_config, const CacheConfig& L2_config,
                    unsigned L1_latency, unsigned L2_latency, unsigned dram_latency);

    unsigned access(addr_t address, unsigned tid);

    std::array<CacheStats, 2> get_L1_stats(void) const;
    std::array<CacheStats, 2> get_L2_stats(void) const;
};
