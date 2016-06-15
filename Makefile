CXXFLAGS=-g -Wall -O2 -std=c++11

all: test_trace

test_trace: TraceRecorder.o TraceReplayer.o
	$(CXX) $(CXXFLAGS) -o test_trace test_trace.cpp TraceRecorder.o TraceReplayer.o

TraceRecorder.o: TraceRecorder.cpp TraceRecorder.h
	$(CXX) $(CXXFLAGS) -c TraceRecorder.cpp

TraceReplayer.o: TraceReplayer.cpp TraceReplayer.h
	$(CXX) $(CXXFLAGS) -c TraceReplayer.cpp

clean:
	rm *.o test_trace dump_trace
