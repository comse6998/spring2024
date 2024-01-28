#include <blas/zcopy.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 zcopy_(i32*, c128*, i32*, c128*, i32*);

const int N = 20;

void test_zcopy(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    i32 incy = (rand() % 16) - 8;

    c128 *x = (c128*)CDC8600::memalloc(n*abs(incx)*2);
    c128 *y = (c128*)CDC8600::memalloc(n*abs(incy)*2);
    c128 *Y = new c128[n*abs(incy)];

    for (int i = 0; i < n*abs(incx); i++) { x[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < n*abs(incy); i++) { y[i] = 0.0;	 }
    for (int i = 0; i < n*abs(incy); i++) { Y[i] = 0.0;	 }

    zcopy_(&n, x, &incx, Y, &incy);		// Reference implementation of DCOPY
    CDC8600::BLAS::zcopy(n, x, incx, y, incy);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    for (int i = 0; i < n*abs(incy); i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }

    delete [] Y;

    cout << "zcopy [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", incy = " << setw(2) << incy << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_zcopy(i);
    }
    return 0;
}
