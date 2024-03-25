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
	cerr << "Usage: " << argv[0] << " <encoding>" << endl;
	return -1;
    }

    instructions::initmakers();

    cout << "Decoding : " << argv[1];

    vector<u64> ops = instructions::decode(strtol(argv[1], NULL, 0));

    cout << " resulted in " << ops.size() << " operations:" << endl;

    for (u32 i=0; i<ops.size(); i++) cout << setfill('0') << setw(16) << hex << ops[i] << dec << setfill(' ') << endl;

    return 0;
}
