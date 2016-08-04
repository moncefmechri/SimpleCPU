#include <iostream>
#include <string>

#include "SimpleCPU.h"

#include <libcachesim/CacheToolbox.h>

using namespace std;


/* Returns the L2 associativity needed in order for L2 to have the same number of sets
 * than L1 */
unsigned compute_L2_associativity(unsigned L1_size, unsigned L1_assoc, unsigned L2_size)
{
    const float ratio = L2_size / (float) L1_size;
    cout << ratio << endl;
    cout << "L2 assoc: " << L1_assoc * ratio << endl;
    return L1_assoc * ratio;
}

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
    const unsigned L2_assoc = compute_L2_associativity(L1_size, L1_assoc, L2_size);

    CacheConfig L1_config(L1_size, L1_assoc, 64);
    CacheConfig L2_config(L2_size, L2_assoc, 64);
    SimpleCPU cpu(argv[1], argv[2], L1_config, L2_config, 1, 1, 10, 130);

    cpu.run();
}
