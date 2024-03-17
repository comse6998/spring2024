#include <blas/dcopy.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t dcopy_(int32_t *, double *, int32_t *, double *, int32_t *);

const u32 N = 20;

void test_dcopy(int count)
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

    for (u32 i = 0; i < nx; i++) { x[i] = drand48(); }
    for (u32 i = 0; i < ny; i++) { y[i] = 0.0;	 }
    for (u32 i = 0; i < ny; i++) { Y[i] = 0.0;	 }

    dcopy_(&n, x, &incx, Y, &incy);		// Reference implementation of DCOPY
    CDC8600::BLAS::dcopy(n, x, incx, y, incy);	// Implementation of DCOPY for the CDC8600

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

    cout << "dcopy [" << setw(2) << count << "] ";
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
    if (n < 10) dump(PROC[0].trace, "dcopy.tr");
}

int main()
{
    for (u32 i = 0; i < N; i++)
    {
        test_dcopy(i);
    }
    return 0;
}
