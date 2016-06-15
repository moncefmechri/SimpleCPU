#include <iostream>

#include "TraceRecorder.h"
#include "TraceReplayer.h"

using namespace std;

int main(void)
{
    const string filename = "/tmp/test_trace.trace";
    TraceRecorder recorder(filename);

    recorder.log_instruction();
    recorder.log_instruction();
    recorder.log_memaccess(0xdeadbeef);
    recorder.log_memaccess(0xbabe);
    recorder.log_instruction();
    recorder.close();

    TraceReplayer replayer(filename);
    while (replayer)
    {
        auto interleaving = replayer.next();
        print_pair(interleaving);
    }
}
