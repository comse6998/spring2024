#include <blas/zaxpy.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 zaxpy_(i32*, c128*, c128*, i32*, c128*, i32*);

const int N = 20;
const double EPSILON = 1e-9;

void test_zaxpy(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    i32 incy = (rand() % 16) - 8;
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 ny = n*abs(incy); if (0 == ny) ny = 1;
    c128 a = c128(drand48(), drand48());
    
    tracing = false; if (n < 10) tracing = true;

    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *y = (c128*)CDC8600::memalloc(ny*2);
    c128 *Y = new c128[ny];

    for (u32 i = 0; i < nx; i++) { x[i] = c128(drand48(), drand48()); }
    for (u32 i = 0; i < ny; i++) { y[i] = c128(drand48(), drand48()); }
    for (u32 i = 0; i < ny; i++) { Y[i] = y[i]; }

    zaxpy_(&n, &a, x, &incx, Y, &incy);		// Reference implementation of ZAXPY
    CDC8600::BLAS::zaxpy(n, a, x, incx, y, incy);	// Implementation of ZAXPY for the CDC8600

    bool pass = true;
    for (u32 i = 0; i < ny; i++)
    {
        if (abs(Y[i] - y[i]) > (min(abs(Y[i]), abs(y[i])) + EPSILON) * EPSILON)
        {
            pass = false;
        }
    }

    delete [] Y;
    CDC8600::memfree(y, ny*2);
    CDC8600::memfree(x, nx*2);

    cout << "zaxpy [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", a = " << setw(20) << a;
    cout << ", incx = " << setw(2) << incx;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (n < 10) {
        dump(PROC[0].trace);
        dump(PROC[0].trace, "zaxpy.tr");
    }
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_zaxpy(i);
    }
    return 0;
}
