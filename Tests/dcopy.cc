#include <blas/dcopy.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>

using namespace CDC8600;

extern "C" int32_t dcopy_(int32_t *, double *, int32_t *, double *, int32_t *);

const int N = 10;

void test_dcopy(int count)
{
    reset();

    int32_t n = 7;
    int32_t incx = 1;
    int32_t incy = 1;

    f64 *x = (f64*)CDC8600::memalloc(n);
    f64 *y = (f64*)CDC8600::memalloc(n);

    for (int i = 0; i < n; i++)
    {
        x[i] = drand48();
    }
    CDC8600::BLAS::dcopy(n, x, incx, y, incy);

    bool pass = true;
    for (int i = 0; i < n; i++)
    {
        if (x[i] != y[i])
        {
            pass = false;
        }
    }
    std::cout << "dcopy [" << count << "] : ";
    if (pass)
        std::cout << "PASS" << std::endl;
    else
        std::cout << "FAIL" << std::endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dcopy(i);
    }
    return 0;
}
