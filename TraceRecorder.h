#pragma once

#include <fstream>

class TraceRecorder
{
    uint64_t current_instruction_count;
    std::ofstream trace_file;
public:
    TraceRecorder(const std::string& filename);
    void log_instruction(void);
    void log_memaccess(uint64_t address);
    ~TraceRecorder(void);
};
