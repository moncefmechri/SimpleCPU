#include <array>

#include "CacheController.h"

#include <libcachesim/ExclusiveCache.h>

CacheController::CacheController(const CacheConfig& L1_config, const CacheConfig& L2_config,
                                 unsigned int L1_latency, unsigned int L2_latency,
                                 unsigned int dram_latency) : hierarchy(L1_config, L2_config)
{
    latencies[static_cast<int>(ExclusiveCache::ACCESS_STATUS::L1_HIT)] = L1_latency;
    latencies[static_cast<int>(ExclusiveCache::ACCESS_STATUS::L1_MISS_L2_HIT)] = L2_latency;
    latencies[static_cast<int>(ExclusiveCache::ACCESS_STATUS::L1_MISS_L2_MISS)] = dram_latency;
}

unsigned CacheController::access(addr_t address, unsigned tid)
{
    const ExclusiveCache::ACCESS_STATUS status = hierarchy.access(address, tid);

    L1_stats[tid].log_access();

    switch (status)
    {
        case ExclusiveCache::ACCESS_STATUS::L1_HIT:
            break;
        case ExclusiveCache::ACCESS_STATUS::L1_MISS_L2_HIT:
            L1_stats[tid].log_miss();
            L2_stats[tid].log_access();
            break;
        case ExclusiveCache::ACCESS_STATUS::L1_MISS_L2_MISS:
            L1_stats[tid].log_miss();
            L2_stats[tid].log_access();
            L2_stats[tid].log_miss();
            break;
    }

    return latencies[static_cast<int>(status)];
}

std::array<CacheStats, 2> CacheController::get_L1_stats(void) const
{
    return L1_stats;
}

std::array<CacheStats, 2> CacheController::get_L2_stats(void) const
{
    return L2_stats;
}
