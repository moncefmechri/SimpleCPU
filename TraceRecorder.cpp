#include <fstream>

#include "TraceRecorder.h"

using namespace std;

TraceRecorder::TraceRecorder(const string& filename) : current_instruction_count(0), trace_file(filename, ios::binary)
{
    if (trace_file.fail())
        throw ios_base::failure("Could not open sample file " + filename);
}

void TraceRecorder::log_instruction(void)
{
    ++current_instruction_count;
}

void TraceRecorder::log_memaccess(uint64_t address)
{
    trace_file.write((const char*) &current_instruction_count, sizeof(current_instruction_count));
    current_instruction_count = 0;
    trace_file.write((const char*) &address, sizeof(address));
}

TraceRecorder::~TraceRecorder(void)
{
    trace_file.write((const char*) &current_instruction_count, sizeof(current_instruction_count));
}

void TraceRecorder::close(void)
{
    trace_file.close();
}
