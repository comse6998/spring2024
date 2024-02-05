#include<stdint.h>
#include<iostream>
#include<iomanip>

using namespace std;

typedef uint64_t	u64;
typedef double		f64;

int main
(
    int		argc,
    char**	argv
)
{
    f64 zero = 0.0;
    f64 pone = 1.0;
    f64 mone = -1.0;
    f64 ptwo = 2.0;
    f64 mtwo = -2.0;
    f64 pinfty = pone/zero;
    f64 minfty = mone/zero;
    f64 nan1 = zero/zero;
    f64 nan2 = zero*pinfty;

    cout << "zero =   " << setw(8) << zero   << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&zero))   << setfill(' ') << endl;
    cout << "pone =   " << setw(8) << pone   << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&pone))   << setfill(' ') << endl;
    cout << "ptwo =   " << setw(8) << ptwo   << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&ptwo))   << setfill(' ') << endl;
    cout << "mone =   " << setw(8) << mone   << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&mone))   << setfill(' ') << endl;
    cout << "mtwo =   " << setw(8) << mtwo   << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&mtwo))   << setfill(' ') << endl;
    cout << "pinfty = " << setw(8) << pinfty << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&pinfty)) << setfill(' ') << endl;
    cout << "minfty = " << setw(8) << minfty << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&minfty)) << setfill(' ') << endl;
    cout << "nan1 =   " << setw(8) << nan1   << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&nan1))   << setfill(' ') << endl;
    cout << "nan2 =   " << setw(8) << nan2   << ", pattern = 0x" << hex << setfill('0') << setw(16) << *((u64*)(&nan2))   << setfill(' ') << endl;

    return 0;
}
