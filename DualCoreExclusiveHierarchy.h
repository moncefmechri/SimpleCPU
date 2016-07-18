#pragma once

#include <libcachesim/CacheToolbox.h>
#include <libcachesim/ExclusiveCache.h>
#include <libcachesim/LRUCache.h>

class DualCoreExclusiveHierarchy
{
    LRUInternals L1_caches[2];
    ExclusiveCache L2;

public:
    DualCoreExclusiveHierarchy(const CacheConfig& L1_config, const CacheConfig& L2_config);
    ExclusiveCache::ACCESS_STATUS access(addr_t address, unsigned tid);
};
