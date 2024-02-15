#include <blas/dasum.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" f64 dasum_(i32*, f64*, i32*);

const int N = 20;
const double epsilon = pow(1, -9);

void test_dasum()
{
    reset();

    int32_t n = rand() % 256;
    int32_t incx = (rand() % 16);
    uint32_t nx = n*abs(incx);  if(0 == nx) nx = 1;

    bool pass = true;

    tracing = false; if (n < 10) tracing = true;

    f64 *x = (f64*)CDC8600::memalloc(nx);

    for (int i = 0; i < nx; i++) {x[i] = drand48(); }

    f64 ref = dasum_(&n, x, &incx);
    f64 ans = CDC8600::BLAS::dasum(n, x, incx);

    if (std::abs(ans-ref) > epsilon*ref) {pass = false; }
    if (pass)
        std::cout << "PASS" << endl;
    else
        std::cout << "FAIL" << endl;

    if (n < 20) dump(trace);
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dasum();
    }
    return 0;
}