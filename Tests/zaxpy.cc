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

    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *y = (c128*)CDC8600::memalloc(ny*2);
    c128 *Y = new c128[ny];

    for (int i = 0; i < nx; i++) { x[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < ny; i++) { y[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < ny; i++) { Y[i] = y[i]; }

    zaxpy_(&n, &a, x, &incx, Y, &incy);		// Reference implementation of ZAXPY
    CDC8600::BLAS::zaxpy(n, a, x, incx, y, incy);	// Implementation of ZAXPY for the CDC8600

    bool pass = true;
    for (int i = 0; i < ny; i++)
    {
        if (abs(Y[i] - y[i]) > (min(abs(Y[i]), abs(y[i])) + EPSILON) * EPSILON)
        {
            pass = false;
        }
    }

    delete [] Y;

    cout << "zaxpy [" << setw(2) << count << "] (n = " << setw(3) << n << ", a = " << setw(20) << a <<  ", incx = " << setw(2) << incx << ", incy = " << setw(2) << incy << ", # of instr = " << setw(9) << instructions::count << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_zaxpy(i);
    }
    return 0;
}
