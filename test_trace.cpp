#include "TraceRecorder.h"

int main(void)
{
    TraceRecorder recorder("/tmp/test_trace.trace");

    recorder.log_instruction();
    recorder.log_instruction();
    recorder.log_memaccess(0xdeadbeef);
    recorder.log_memaccess(0xbabe);
    recorder.log_instruction();
}
