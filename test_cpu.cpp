#include <iostream>
#include "SimpleCPU.h"

#include <libcachesim/CacheToolbox.h>


using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cerr << "Must provide two filenames as arguments" << endl;
        exit(1);
    }

    CacheConfig L1_config(8*1024, 64, 64);
    CacheConfig L2_config(L1_config);
    SimpleCPU cpu(argv[1], argv[2], L1_config, L2_config, 1, 1, 1, 50);


    cpu.run();
}
