#include "CacheController.h"

#include <libcachesim/ExclusiveCache.h>

CacheController::CacheController(const CacheConfig& L1_config, const CacheConfig& L2_config,
                                 unsigned int L1_latency, unsigned int L2_latency,
                                 unsigned int dram_latency) : hierarchy(L1_config, L2_config)
{
    latencies[ExclusiveCache::ACCESS_STATUS::L1_HIT] = L1_latency;
    latencies[ExclusiveCache::ACCESS_STATUS::L1_MISS_L2_HIT] = L2_latency;
    latencies[ExclusiveCache::ACCESS_STATUS::L1_MISS_L2_MISS] = dram_latency;
}

unsigned CacheController::access(addr_t address, unsigned tid)
{
    const ExclusiveCache::ACCESS_STATUS status = hierarchy.access(address, tid);

    return latencies[status];
}
