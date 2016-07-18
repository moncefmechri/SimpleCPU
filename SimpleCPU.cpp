#include <iostream>
#include <vector>

#include "SimpleCPU.h"


SimpleCPU::SimpleCPU(const std::string& filename_0, const std::string& filename_1,
                     const CacheConfig& L1_config, const CacheConfig& L2_config,
                     unsigned int instruction_latency, unsigned int L1_latency,
                     unsigned int L2_latency, unsigned int dram_latency)
    : replayer_0(filename_0), replayer_1(filename_1), instruction_latency(instruction_latency),
      cache_controller(L1_config, L2_config, L1_latency, L2_latency, dram_latency)
{
    instruction_count[0] = 0;
    instruction_count[1] = 0;

    mem_accesses[0] = 0;
    mem_accesses[1] = 0;

    cycles[0] = 0;
    cycles[1] = 0;
}

void SimpleCPU::run(void)
{
    int64_t delay = 0;

    record_type record_0;
    record_type record_1;
    std::vector<uint64_t>::iterator it_0, it_end_0, it_1, it_end_1;

    if (replayer_0)
    {
        record_0 = replayer_0.next();

        delay += record_0.first * instruction_latency;
        cycles[0] += record_0.first * instruction_latency;
        instruction_count[0] += record_0.first;

        it_0 = record_0.second.begin();
        it_end_0 = record_0.second.end();
    }

    if (replayer_1)
    {
        record_1 = replayer_1.next();

        delay -= record_1.first * instruction_latency;
        cycles[1] += record_1.first * instruction_latency;
        instruction_count[1] += record_1.first;

        it_1 = record_1.second.begin();
        it_end_1 = record_1.second.end();
    }

    bool core_0_completed = false;
    bool core_1_completed = false;

    while (!core_0_completed && !core_1_completed)
    {
        //WARNING: For now, in case of tie, core 0 always goes first
        while (it_0 != it_end_0 && delay <= 0)
        {
            const unsigned latency = cache_controller.access(*it_0, 0);
            delay += latency;
            cycles[0] += latency;
            ++it_0;

            ++mem_accesses[0];
        }

        //Check if we exited because we consumed all the mem accesses in the current record. Reload a new one,
        //or exit the infinite loop if there is no more record for this core
        if (it_0 == it_end_0)
        {
            if (replayer_0)
            {
                record_0 = replayer_0.next();

                delay += record_0.first * instruction_latency;
                cycles[0] += record_0.first * instruction_latency;
                instruction_count[0] += record_0.first;

                it_0 = record_0.second.begin();
                it_end_0 = record_0.second.end();
            }

            else
                core_0_completed = true;
        }

        while (it_1 != it_end_1 && delay > 0)
        {
            const unsigned latency = cache_controller.access(*it_1, 1);
            delay -= latency;
            cycles[1] += latency;
            ++it_1;
            ++mem_accesses[1];
        }

        if (it_1 == it_end_1)
        {
            if (replayer_1)
            {
                record_1 = replayer_1.next();
                delay -= record_1.first * instruction_latency;
                cycles[1] += record_1.first * instruction_latency;
                instruction_count[1] += record_1.first;

                it_1 = record_1.second.begin();
                it_end_1 = record_1.second.end();

            }
            else
                core_1_completed = true;
        }
    }

    //Finish processing the remaining instructions, if any

    if (!core_0_completed)
    {
        //Finish processing the current memory instructions, if any, before treating the remaining records
        while (it_0 != it_end_0)
        {
            const unsigned latency = cache_controller.access(*it_0, 0);
            cycles[0] += latency;
            ++it_0;
            ++mem_accesses[0];
        }

        while (replayer_0)
        {
            record_0 = replayer_0.next();

            cycles[0] += record_0.first * instruction_latency;
            instruction_count[0] += record_0.first;

            for (it_0 = record_0.second.begin(); it_0 != record_0.second.end(); ++it_0)
            {
                const unsigned latency = cache_controller.access(*it_0, 0);
                cycles[0] += latency;
                ++mem_accesses[0];
            }
        }
    }

    if (!core_1_completed)
    {
        //Finish processing the current memory instructions, if any, before treating the remaining records
        while (it_1 != it_end_1)
        {
            const unsigned latency = cache_controller.access(*it_1, 1);
            cycles[1] += latency;
            ++it_1;
            ++mem_accesses[1];
        }

        while (replayer_1)
        {
            record_1 = replayer_1.next();

            cycles[1] += record_1.first * instruction_latency;
            instruction_count[1] += record_1.first;

            for (it_1 = record_1.second.begin(); it_1 != record_1.second.end(); ++it_1)
            {
                const unsigned latency = cache_controller.access(*it_1, 1);
                cycles[1] += latency;
                ++mem_accesses[1];
            }
        }
    }
    print_statistics();
}

void SimpleCPU::print_statistics(std::ostream& os)
{
    auto L1_stats = cache_controller.get_L1_stats();
    CacheStats L2_stats = cache_controller.get_L2_stats();

    os << "==Core 0==" << std::endl;
    os << "Instructions: " << instruction_count[0] << std::endl;
    os << "Memory accesses: " << mem_accesses[0] << std::endl;
    os << "Cycles: " << cycles[0] << " (CPI: " << cycles[0] / (float) instruction_count[0] << ')' << std::endl;
    os << "L1 stats:" << std::endl;
    print_cache_stats(L1_stats[0], os);
    os << std::endl;

    os << "==Core 1==" << std::endl;
    os << "Instructions: " << instruction_count[1] << std::endl;
    os << "Memory accesses: " << mem_accesses[1] << std::endl;
    os << "Cycles: " << cycles[1] << " (CPI: " << cycles[1] / (float) instruction_count[1] << ')' << std::endl;
    os << "L1 stats:" << std::endl;

    print_cache_stats(L1_stats[1], os);
    os << std::endl;

    os << "==Shared L2==" << std::endl;
    print_cache_stats(L2_stats, os);
}

