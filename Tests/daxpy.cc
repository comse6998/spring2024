#include <blas/daxpy.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 daxpy_(int32_t *, double *, double *, int32_t *, double *, int32_t *);

const int N = 20;
const double EPSILON = 1e-9;

void test_daxpy(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    i32 incy = (rand() % 16) - 8;
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 ny = n*abs(incy); if (0 == ny) ny = 1;
    f64 a = f64(drand48());

    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *y = (f64*)CDC8600::memalloc(ny);
    f64 *Y = new f64[ny];

    for (int i = 0; i < nx; i++) { x[i] = f64(drand48()); }
    for (int i = 0; i < ny; i++) { y[i] = f64(drand48()); }
    for (int i = 0; i < ny; i++) { Y[i] = y[i]; }

    daxpy_(&n, &a, x, &incx, Y, &incy);		// Reference implementation of DAXPY
    CDC8600::BLAS::daxpy(n, a, x, incx, y, incy);	// Implementation of DAXPY for the CDC8600

    bool pass = true;
    for (int i = 0; i < ny; i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }

    delete [] Y;

    cout << "daxpy [" << setw(2) << count << "] (n = " << setw(3) << n << ", a = " << setw(20) << a <<  ", incx = " << setw(2) << incx << ", incy = " << setw(2) << incy << ", # of instr = " << setw(9) << instructions::count << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_daxpy(i);
    }
    return 0;
}
