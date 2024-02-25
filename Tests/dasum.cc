#include <blas/dasum.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace CDC8600;

extern "C" f64 dasum_(i32*, f64*, i32*);

const int N = 20;
const double epsilon = 1e-9;

void test_dasum(int count)
{
    reset();

    bool pass = true;
    int32_t n = rand() % 256;
    int32_t incx = (rand() % 16);
    uint32_t nx = n*abs(incx);
    if(0 == nx) nx = 1;

    tracing = n < 15;

    f64 *x = (f64*)CDC8600::memalloc(nx);

    for (int i = 0; i < nx; i++) {x[i] = drand48(); }

    f64 ref = dasum_(&n, x, &incx);
    f64 ans = CDC8600::BLAS::dasum(n, x, incx);

    if (std::abs(ans-ref) > epsilon*ref) {pass = false; }
    cout << "dasum [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", # of instr = " << setw(9) << instructions::count << ") : ";
    if (pass)
        std::cout << "PASS" << endl;
    else
        std::cout << "FAIL" << endl;

    if (n<15) dump(trace);
}

int main()
{
    // srand(time(NULL));  // If we want smth new.
    for (int i = 0; i < N; i++) test_dasum(i);

    return 0;
}