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

void test_zswap(int count, bool inc_one)
{
    reset();

    // test special case of incx = 1 and incy = 1
    i32 n = 5; 
    i32 incx = 1; 
    i32 incy = 1;
    if(!inc_one)
    {
        n = rand() % 256;
        incx = (rand() % 16) - 8;
        incy = (rand() % 16) - 8;
    }
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 ny = n*abs(incy); if (0 == ny) ny = 1;

    tracing = false; if (n < 10) tracing = true;
    
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
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (n < 10) dump(PROC[0].trace);
    if (n < 10) dump(PROC[0].trace, "zswap.tr");

}

int main()
{
    
    for (unsigned i = 0; i < N; i++)
    {
        test_zswap(i, false);
    }
    test_zswap(N, true);
    return 0;
}
