#include <blas/idamax.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 idamax_(i32*, double*, i32*);

const int N = 50;

void test_idamax(int count, bool traceon, i32 n, i32 incx)
{
    reset();

    double *x = (double*)CDC8600::memalloc(n*abs(incx));

    for (int i = 0; i < n*abs(incx); i++) { x[i] = double(drand48()); }

    tracing = traceon;

    i64 index_ = idamax_(&n, x, &incx);		
    i64 index = CDC8600::BLAS::idamax(n, x, incx);	
    bool pass = true;
    if (index_ != index)
    {
        pass = false;
    }
    cout << "dcopy [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", # of instr = ";
    for (u32 i = 0; i < params::Proc::N; i++)cout << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = ";
    for (u32 i = 0; i < params::Proc::N; i++)cout << setw(9) << PROC[0].op_maxcycle;
    
    cout << ") : ";

    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (traceon) dump(PROC[0].trace, "idamax.tr");

}

int main( 
    int		argc,
    char	**argv
    )
{
    if (1 == argc)
    {
	for (u32 i = 0; i < N; i++)
	{
	    i32 n = rand() % 256;
	    i32 incx = (rand() % 16) - 8;
	    i32 incy = (rand() % 16) - 8;
	    test_idamax(i, false, n, incx);
	}
    }
    else if (3 == argc)
    {
	i32 n = atoi(argv[1]);
	i32 incx = atoi(argv[2]);
	test_idamax(0, true, n, incx);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [n incx]" << endl;
	return -1;
    }
    return 0;
}
