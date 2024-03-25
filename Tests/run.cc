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

    instructions::initmakers();
    pipeline::reset();
    pipeline::run(argv[1]);

    return 0;
}
