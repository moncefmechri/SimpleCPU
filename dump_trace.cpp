#include "TraceReplayer.h"

int main(int argc, char** argv)
{
    TraceReplayer replayer(argv[1]);

    while (replayer)
    {
        print_pair(replayer.next());
    }
}
