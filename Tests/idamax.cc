#include <blas/idamax.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 idamax_(i32*, double*, i32*);

const int N = 50;

void test_idamax(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16)-4;

    double *x = (double*)CDC8600::memalloc(n*abs(incx));

    for (int i = 0; i < n*abs(incx); i++) { x[i] = double(drand48()); }

    tracing = false; if (n < 10) tracing = true;

    i64 index_ = idamax_(&n, x, &incx);		// Reference implementation of DCOPY
    i64 index = CDC8600::BLAS::idamax(n, x, incx);	// Implementation of DCOPY for the CDC8600
    bool pass = true;
    if (index_ != index)
    {
        pass = false;
    }
    cout << "dcopy [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
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
        test_idamax(i);
    }
    return 0;
}
