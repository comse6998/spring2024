#include <blas/zdrot.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 zdrot_(i32 *, c128 *, i32 *, c128 *, i32 *, f64 *, f64 *);

const int N = 20;

void test_zdrot(int count)
{
    reset();
    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    i32 incy = (rand() % 16) - 8;
    if( count == 19 ){
        n = 3;
        incx = 1;
        incy = 1;
    }
    if (incx == 0) incx = 1;
    if (incy == 0) incy = 1;

    c128 *x = (c128*)CDC8600::memalloc(n*abs(incx)*2);
    c128 *y = (c128*)CDC8600::memalloc(n*abs(incy)*2);
    c128 *X = new c128[n*abs(incx)];
    c128 *Y = new c128[n*abs(incy)];
    f64 c = 0.758;
    f64 s = 0.111;

    for (int i = 0; i < n*abs(incx); i++) { x[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < n*abs(incy); i++) { y[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < n*abs(incx); i++) { X[i] = x[i]; }
    for (int i = 0; i < n*abs(incy); i++) { Y[i] = y[i]; }

    tracing = false;
    if (n < 10) tracing = true;

 
    CDC8600::BLAS::zdrot(n, x, incx, y, incy, c, s);	// Implementation of ZDROT for the CDC8600
    zdrot_(&n, X, &incx, Y, &incy, &c, &s);		// Reference implementation of ZDROT

    bool pass = true;
    for (int i = 0; i < n*abs(incy); i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }
    for (int i = 0; i < n*abs(incx); i++)
    {
        if (X[i] != x[i])
        {
            pass = false;
        }
    }

    delete [] Y;
    delete [] X;

    cout << "zdrot [" << setw(2) << count << "] ";
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", incy = " << setw(3) << incy;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (n < 10) dump(PROC[0].trace);
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_zdrot(i);
    }
    return 0;
}
