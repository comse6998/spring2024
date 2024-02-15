#include <blas/idamax.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 idamax_(i32*, double*, i32*);

const int N = 100;

void test_idamax(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16)-4;

    double *x = (double*)CDC8600::memalloc(n*abs(incx));

    for (int i = 0; i < n*abs(incx); i++) { x[i] = double(drand48()); }

    i64 index_ = idamax_(&n, x, &incx);		// Reference implementation of DCOPY
    i64 index = CDC8600::BLAS::idamax(n, x, incx);	// Implementation of DCOPY for the CDC8600
    printf("fortran: %d, asm: %d\n", index_, index);
    bool pass = true;
    if (index_ != index)
    {
        pass = false;
    }

    cout << "idamax [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_idamax(i);
    }
    return 0;
}
