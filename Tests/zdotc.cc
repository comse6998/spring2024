#include <blas/zdotc.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" c128 zdotc_(i32*, c128*, i32*, c128*, i32*);

const int N = 20;
const double epsilon = 1e-9;
void test_zdotc(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    i32 incy = (rand() % 16) - 8;
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 ny = n*abs(incy); if (0 == ny) ny = 1;

    tracing = false; if (n < 10) tracing = true;

    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *y = (c128*)CDC8600::memalloc(ny*2);

    for (u32 i = 0; i < nx; i++) { x[i] = c128(drand48(), drand48()); }
    for (u32 i = 0; i < ny; i++) { y[i] = c128(drand48(), drand48()); }
    
    c128 Z = zdotc_(&n, x, &incx, y, &incy);		// Reference implementation of ZDOTC
    c128 z = CDC8600::BLAS::zdotc(n, x, incx, y, incy);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    if(!(abs(z.real() - Z.real()) < (min(abs(z.real()), abs(Z.real())) + epsilon) * epsilon))
    {
        pass = false;
    }

    if(!(abs(z.imag() - Z.imag()) < (min(abs(z.imag()), abs(Z.imag())) + epsilon) * epsilon))
    {
        pass = false;
    }

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
    
    if (n < 10) dump(PROC[0].trace, "zdotc.tr");
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_zdotc(i);
    }
    return 0;
}
