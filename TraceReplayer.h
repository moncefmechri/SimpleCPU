#pragma once

#include <fstream>
#include <iterator>
#include <iostream>
#include <utility>
#include <vector>

using record_type = std::pair<uint64_t, std::vector<uint64_t>>;

class TraceReplayer
{
    std::ifstream trace_file;
    record_type next_record;
    bool clean_eof;

    record_type read_next_record(void);

public:
    TraceReplayer(const std::string& filename);
    record_type next(void);
    operator bool() const;
};

void print_pair(const record_type& interleaving);

template <typename T>
T read(std::istream& is)
{
    T value;
    is.read(reinterpret_cast<char*>(&value), sizeof(T));

    return value;
}
