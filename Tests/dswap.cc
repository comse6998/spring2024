#include <blas/dswap.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t dswap_(int32_t *, double *, int32_t *, double *, int32_t *);

const int N = 20;


void test_dswap(int count)
{
    reset();

    int32_t n = rand() % 256;
    int32_t incx = (rand() % 16) - 8;
    int32_t incy = (rand() % 16) - 8;
    uint32_t nx = n*abs(incx); if (0 == nx) nx = 1;
    uint32_t ny = n*abs(incy); if (0 == ny) ny = 1;
    
    tracing = false; if (n < 10) tracing = true;

    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *y = (f64*)CDC8600::memalloc(ny);
    f64 *Y = new f64[ny];
    f64 *X = new f64[nx];

    for (int i = 0; i < nx; i++) { 
        x[i] = drand48();
        X[i] = x[i]; }

    for (int i = 0; i < ny; i++) { 
        y[i] = drand48();
        Y[i] = y[i]; }

    dswap_(&n, X, &incx, Y, &incy);		// Reference implementation of DCOPY
    CDC8600::BLAS::dswap(n, x, incx, y, incy);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    for (int i = 0; i < ny; i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }

    for (int i = 0; i < nx; i++)
    {
        if (X[i] != x[i])
        {
            pass = false;
        }
    }

    delete [] Y;

    
    
    cout << "dswap [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", incy = " << setw(2) << incy << ", # of instr = " << setw(9) << instructions::count << ") : ";
    //cout << "dcopy [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = " << setw(9) << instructions::count;
    cout << ", # of cycles = " << setw(9) << operations::maxcycle;
    cout << ") : ";
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
        test_dswap(i);
    }
    return 0;
}
