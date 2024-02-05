#include <blas/dcopy.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t dcopy_(int32_t *, double *, int32_t *, double *, int32_t *);

const int N = 20;

void test_dcopy(int count)
{
    reset();

    int32_t n = rand() % 256;
    int32_t incx = (rand() % 16) - 8;
    int32_t incy = (rand() % 16) - 8;

    f64 *x = (f64*)CDC8600::memalloc(n*abs(incx));
    f64 *y = (f64*)CDC8600::memalloc(n*abs(incy));
    f64 *Y = new f64[n*abs(incy)];

    for (int i = 0; i < n*abs(incx); i++) { x[i] = drand48(); }
    for (int i = 0; i < n*abs(incy); i++) { y[i] = 0.0;	 }
    for (int i = 0; i < n*abs(incy); i++) { Y[i] = 0.0;	 }

    dcopy_(&n, x, &incx, Y, &incy);		// Reference implementation of DCOPY
    CDC8600::BLAS::dcopy(n, x, incx, y, incy);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    for (int i = 0; i < n*abs(incy); i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }

    delete [] Y;

    cout << "dcopy [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", incy = " << setw(2) << incy << ", # of instr = " << setw(9) << instructions::count << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (n < 10) dump(trace);
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dcopy(i);
    }
    return 0;
}
