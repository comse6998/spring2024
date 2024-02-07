#include <blas/zdotc.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 zdotc_(i32*, c128*, i32*, c128*, i32*, c128*);

const int N = 20;

void test_zdotc(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    i32 incy = (rand() % 16) - 8;
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 ny = n*abs(incy); if (0 == ny) ny = 1;

    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *y = (c128*)CDC8600::memalloc(ny*2);
    c128 *z = (c128*)CDC8600::memalloc(2);
    c128 *Z = new c128[1];

    for (int i = 0; i < nx; i++) { x[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < ny; i++) { y[i] = c128(drand48(), drand48()); }
    *z = 0.0;
    *Z = 0.0;

    zdotc_(&n, x, &incx, y, &incy, Z);		// Reference implementation of DCOPY
    CDC8600::BLAS::zdotc(n, x, incx, y, incy, z);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    cout<<"z : "<<*z<<"\n";
    if(*z != *Z)
    {
        pass = false;
    }

    delete [] Z;

    cout << "zdotc [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", incy = " << setw(2) << incy << ", # of instr = " << setw(9) << instructions::count << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_zdotc(i);
    }
    return 0;
}
