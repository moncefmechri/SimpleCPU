#include <stdexcept>

#include "DualCoreExclusiveHierarchy.h"

DualCoreExclusiveHierarchy::DualCoreExclusiveHierarchy(const CacheConfig& L1_config,
                                                       const CacheConfig& L2_config)
    : L1_caches {L1_config, L1_config}, L2(L2_config) {}


ExclusiveCache::ACCESS_STATUS DualCoreExclusiveHierarchy::access(addr_t address, unsigned tid)
{
    if (tid > 1)
        throw std::runtime_error("Invalid tid");

    return L2.access(address, L1_caches[tid], tid);
}
