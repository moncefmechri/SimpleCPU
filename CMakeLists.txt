set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_library(TraceRecorder OBJECT TraceRecorder.cpp)
add_library(TraceReplayer OBJECT TraceReplayer.cpp)

add_executable(dump_trace $<TARGET_OBJECTS:TraceReplayer> dump_trace.cpp)
add_executable(test_trace $<TARGET_OBJECTS:TraceRecorder> $<TARGET_OBJECTS:TraceReplayer> test_trace.cpp)
