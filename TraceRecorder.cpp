#include <fstream>

#include "TraceRecorder.h"

using namespace std;

TraceRecorder::TraceRecorder(const string& filename) : current_instruction_count(0), trace_file(filename, ios::binary)
{
    if (trace_file.fail())
        throw ios_base::failure("Could not open trace file " + filename);
}

void TraceRecorder::log_instruction(void)
{
    if (!addresses.empty())
    {
        write(trace_file, current_instruction_count);
        write(trace_file, (uint64_t) addresses.size());

        for (auto address : addresses)
            write(trace_file, address);

        addresses.clear();
        current_instruction_count = 0;
    }

    ++current_instruction_count;
}

void TraceRecorder::log_memaccess(uint64_t address)
{
    addresses.push_back(address);
}

TraceRecorder::~TraceRecorder(void)
{
    if (trace_file.is_open())
        close();
}

void TraceRecorder::close(void)
{
    write(trace_file, current_instruction_count);
    write(trace_file, (uint64_t) addresses.size());
    for (auto address : addresses)
        write(trace_file, address);
    trace_file.close();
}
