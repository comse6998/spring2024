#include <blas/dasum.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" f64 dasum_(i32*, f64*, i32*);

const int N = 20;
const double epsilon = pow(1, -5);

void test_dasum()
{
    reset();

    int32_t n = rand() % 256;
    int32_t incx = (rand() % 16);
    uint32_t nx = n*abs(incx);  if(0 == nx) nx = 1;
    bool pass = true;

    // tracing = false;

    f64 *dx = (f64*)CDC8600::memalloc(nx);

    for (int i = 0; i < nx; i++) {dx[i] = drand48(); }

    f64 ref = dasum_(&n, dx, &incx);
    f64 ans = CDC8600::BLAS::dasum(n, dx, incx);

    if ((ans-ref) > epsilon*ref) {pass = false; }

    //cout << "dasum [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", incy = " << setw(2) << incy << ", # of instr = " << setw(9) << instructions::count << ") : ";
    if (n < 10) dump(trace);
    if (pass)
        cout << "PASS" << endl;
    else
        cout << "FAIL" << endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dasum();
    }
    return 0;
}