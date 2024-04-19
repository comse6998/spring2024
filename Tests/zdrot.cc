#include <blas/zdrot.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 zdrot_(i32 *, c128 *, i32 *, c128 *, i32 *, f64 *, f64 *);

const int N = 20;

void test_zdrot(int count, bool traceon, i32 n, i32 incx, i32 incy)
{
    reset();

    tracing = traceon;

    if (incx == 0) incx = 1;
    if (incy == 0) incy = 1;

    c128 *x = (c128*)CDC8600::memalloc(n*abs(incx)*2);
    c128 *y = (c128*)CDC8600::memalloc(n*abs(incy)*2);
    c128 *X = new c128[n*abs(incx)];
    c128 *Y = new c128[n*abs(incy)];
    f64 c = 0.758;
    f64 s = 0.111;

    for (int i = 0; i < n*abs(incx); i++) { x[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < n*abs(incy); i++) { y[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < n*abs(incx); i++) { X[i] = x[i]; }
    for (int i = 0; i < n*abs(incy); i++) { Y[i] = y[i]; }
 
    CDC8600::BLAS::zdrot(n, x, incx, y, incy, c, s);	// Implementation of ZDROT for the CDC8600
    zdrot_(&n, X, &incx, Y, &incy, &c, &s);		// Reference implementation of ZDROT

    bool pass = true;
    for (int i = 0; i < n*abs(incy); i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }
    for (int i = 0; i < n*abs(incx); i++)
    {
        if (X[i] != x[i])
        {
            pass = false;
        }
    }

    delete [] Y;
    delete [] X;

    cout << "zdrot [" << setw(2) << count << "] ";
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", incy = " << setw(3) << incy;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;


    if (traceon) dump(PROC[0].trace);
    if (traceon) dump(PROC[0].trace, "zdrot.tr");
    cout << traceon << endl;
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
	    test_zdrot(i, false, n, incx, incy);
	}
    }
    else if (4 == argc)
    {
	i32 n = atoi(argv[1]);
	i32 incx = atoi(argv[2]);
	i32 incy = atoi(argv[3]);
    test_zdrot(0, true, n, incx, incy);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [n incx incy]" << endl;
	return -1;
    }
    return 0;
}
