#include <iostream>
#include <string>

#include "SimpleCPU.h"

#include <libcachesim/CacheToolbox.h>


using namespace std;

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        cerr << "Must provide two filenames and two cache sizes (L1 and L2) as arguments" << endl;
        exit(1);
    }

    const unsigned L1_size = stoul(argv[3]);
    const unsigned L2_size = stoul(argv[4]);

    const unsigned L1_assoc = 64;
    const unsigned L2_assoc = 2 * L1_assoc;

    CacheConfig L1_config(L1_size, L1_assoc, 64);
    CacheConfig L2_config(L2_size, L2_assoc, 64);
    SimpleCPU cpu(argv[1], argv[2], L1_config, L2_config, 1, 1, 1, 50);

    cpu.run();
}
