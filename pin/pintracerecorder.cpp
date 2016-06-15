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

KNOB<string> KnobOut(KNOB_MODE_WRITEONCE, "pintool", "output", "output.trace", "Trace file ");

unique_ptr<TraceRecorder> recorder;

using namespace std;

void fini(int code, void* v)
{}

void instruction(INS ins, void* v)
{}

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
