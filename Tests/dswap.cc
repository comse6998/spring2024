#include <blas/dswap.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t dswap_(int32_t *, double *, int32_t *, double *, int32_t *);

const u32 N = 20;

void test_dswap(int count, bool traceon, i32 n, i32 incx, i32 incy)
{
    reset();

    uint32_t nx = n*abs(incx); if (0 == nx) nx = 1;
    uint32_t ny = n*abs(incy); if (0 == ny) ny = 1;
    
    tracing = traceon;

    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *y = (f64*)CDC8600::memalloc(ny);
    f64 *Y = new f64[ny];
    f64 *X = new f64[nx];

    for (u32 i = 0; i < nx; i++) { x[i] = X[i] = drand48(); }
    for (u32 i = 0; i < ny; i++) { y[i] = 0.0;	 }
    for (u32 i = 0; i < ny; i++) { Y[i] = 0.0;	 }

    dswap_(&n, X, &incx, Y, &incy);		// Reference implementation of DCOPY
    CDC8600::BLAS::dswap(n, x, incx, y, incy);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    for (u32 i = 0; i < ny; i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }

    delete [] Y;
    CDC8600::memfree(y, ny);
    CDC8600::memfree(x, nx);
    cout<< "------------------------------------------------------------------------------------------------------------------------------------\n";
    cout << "dswap [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = "  << setw(9) << PROC[0].instr_count;
    cout << ", # of ops = "    << setw(9) << PROC[0].op_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (traceon) dump(PROC[0].trace);
    if (traceon) dump(PROC[0].trace, "dswap.tr");
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
	    test_dswap(i, false, n, incx, incy);
	}
    }
    else if (4 == argc)
    {
	i32 n = atoi(argv[1]);
	i32 incx = atoi(argv[2]);
	i32 incy = atoi(argv[3]);
	test_dswap(0, true, n, incx, incy);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [n incx incy]" << endl;
	return -1;
    }
    return 0;
}
