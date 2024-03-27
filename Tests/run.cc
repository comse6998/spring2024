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
    int 	argc,
    char**	argv
)
{
    if (2 != argc)
    {
	cerr << "Usage: " << argv[0] << " <trace file>" << endl;
	return -1;
    }

    reset();				// reset the entire computer
    instructions::initmakers();		// initialize the instruction makers
    operations::initmappers();		// initialize the register mappers
    pipeline::reset();			// reset the simulated pipeline
    pipeline::run(argv[1]);		// run the trace through the pipeline

    return 0;
}
