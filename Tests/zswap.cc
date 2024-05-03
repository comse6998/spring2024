#include <blas/zswap.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 zswap_(i32*, c128*, i32*, c128*, i32*);

const int N = 20;

void test_zswap(int count, bool traceon, i32 n, i32 incx, i32 incy)
{
    reset();
    
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 ny = n*abs(incy); if (0 == ny) ny = 1;

    tracing = traceon;
    
    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *X = new c128[nx];
    c128 *y = (c128*)CDC8600::memalloc(ny*2);
    c128 *Y = new c128[ny];

    for (unsigned i = 0; i < nx; i++) { x[i] = c128(drand48(), drand48()); X[i] = x[i]; }
    for (unsigned i = 0; i < ny; i++) { y[i] = c128(drand48(), drand48()); Y[i] = y[i]; }
    
    zswap_(&n, X, &incx, Y, &incy);		// Reference implementation of ZSWAP
    CDC8600::BLAS::zswap(n, x, incx, y, incy);	// Implementation of ZSWAP for the CDC8600

    bool pass = true;
    for (unsigned i = 0; i < nx; i++)
    {
        if (X[i] != x[i])
        {
            pass = false;
        }
    }

    for (unsigned i = 0; i < ny; i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }

    delete [] X;
    delete [] Y;

    cout << "zswap [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of ops = "    << setw(9) << PROC[0].op_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (traceon) dump(PROC[0].trace);
    if (traceon) dump(PROC[0].trace, "zswap.tr");

}

int main
(
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
	    test_zswap(i, false, n, incx, incy);
	}
    }
    else if (4 == argc)
    {
	i32 n = atoi(argv[1]);
	i32 incx = atoi(argv[2]);
	i32 incy = atoi(argv[3]);
	test_zswap(0, true, n, incx, incy);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [n incx incy]" << endl;
	return -1;
    }
    return 0;
}
