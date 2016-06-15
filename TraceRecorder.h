#pragma once


#include <fstream>
#include <iostream>
#include <vector>

class TraceRecorder
{
    uint64_t current_instruction_count;
    std::vector<uint64_t> addresses;
    std::ofstream trace_file;

public:
    TraceRecorder(const std::string& filename);
    void log_instruction(void);
    void log_memaccess(uint64_t address);
    ~TraceRecorder(void);
    void close(void);
};

template <typename T>
void write(std::ostream& os, const T& value)
{
    os.write(reinterpret_cast<const char*> (&value), sizeof(T));
}
