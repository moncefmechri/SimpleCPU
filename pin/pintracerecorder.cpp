#include <memory>

#include "../TraceRecorder.h"

#include "pin.H"
#include "pinplay.H"

PINPLAY_ENGINE pinplay_engine;
KNOB<BOOL> KnobPinPlayLogger(KNOB_MODE_WRITEONCE,
                      "pintool", "log", "0",
                      "Activate the pinplay logger");
KNOB<BOOL> KnobPinPlayReplayer(KNOB_MODE_WRITEONCE,
                      "pintool", "replay", "0",
                      "Activate the pinplay replayer");

KNOB<string> KnobOut(KNOB_MODE_WRITEONCE, "pintool", "o", "output.trace", "Trace file ");

unique_ptr<TraceRecorder> recorder;

using namespace std;

void log_memaccess(void* address)
{
    recorder->log_memaccess((uint64_t) address);
}

void log_instruction(void)
{
    recorder->log_instruction();
}

void fini(int code, void* v)
{
    recorder->close();
}

void instruction(INS ins, void* v)
{
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR) log_instruction, IARG_END);

    const int nb_mem_operands = INS_MemoryOperandCount(ins);
    for (int mem_op = 0; mem_op < nb_mem_operands; ++mem_op)
    {
        if (INS_MemoryOperandIsRead(ins, mem_op))
        {
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR) log_memaccess, IARG_MEMORYOP_EA, mem_op, IARG_END);
        }
        //An instruction could both read and write memory so we always test for both
        if (INS_MemoryOperandIsWritten(ins, mem_op))
        {
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR) log_memaccess, IARG_MEMORYOP_EA, mem_op, IARG_END);
        }
    }
}

int main(int argc, char** argv)
{
    PIN_Init(argc, argv);
    pinplay_engine.Activate(argc, argv,
          KnobPinPlayLogger, KnobPinPlayReplayer);

    recorder.reset(new TraceRecorder(KnobOut.Value()));

    INS_AddInstrumentFunction(instruction, 0);
    PIN_AddFiniFunction(fini, 0);

    PIN_StartProgram();
}
