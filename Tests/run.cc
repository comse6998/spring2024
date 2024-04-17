#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <CDC8600.hh>

using namespace CDC8600;
using namespace std;

int main
(
    int         argc,
    char**      argv
)
{
    if ((2 != argc) && (3 != argc))
    {
        cerr << "Usage: " << argv[0] << " <trace file> [max # cycles]" << endl;
        return -1;
    }

    reset();                                                    // reset the entire computer
    instructions::initmakers();                                 // initialize the instruction makers
    operations::initmappers();                                  // initialize the register mappers
    pipeline::reset();                                          // reset the simulated pipeline
    if (2 == argc) pipeline::run(argv[1], UINT32_MAX);          // run the trace through the pipeline until completion
    if (3 == argc) pipeline::run(argv[1], atoi(argv[2]));       // run the trace through the pipeline for a maximum of argv[2] cycles

    return 0;
}
