#include <fstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <utility>

#include "TraceReplayer.h"

using namespace std;

TraceReplayer::TraceReplayer(const string& filename) : trace_file(filename, ios::binary), clean_eof(false)
{
    if (trace_file.fail())
        throw ios_base::failure("Could not open input file " + filename);

    next_record = read_next_record();
}

record_type TraceReplayer::next(void)
{
    record_type old_record = next_record;
    next_record = read_next_record();

    return old_record;
}

TraceReplayer::operator bool() const
{
    return !clean_eof;
}

void print_pair(const record_type& interleaving)
{
    cout << "Instructions: " << interleaving.first << endl;
    cout << "Memory address(es): ";
    if (interleaving.second.empty())
        cout << "None" << endl;

    else
    {
        cout << hex;
        for (auto address : interleaving.second)
            cout << address << ' ';

        cout << dec << endl;
    }
}

//TODO:Check for premature EOF
record_type TraceReplayer::read_next_record(void)
{
    record_type record;
    clean_eof = trace_file.peek() == EOF;
    if (!clean_eof)
    {
        const uint64_t instruction_count = read<uint64_t>(trace_file);
        const uint64_t nb_mem_accesses = read<uint64_t>(trace_file);

        record.first = instruction_count;

        for (uint64_t i = 0; i < nb_mem_accesses; ++i)
        {
            const uint64_t address = read<uint64_t>(trace_file);
            record.second.push_back(address);
        }
    }

    return record;
}
