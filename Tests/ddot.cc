#include <blas/ddot.hh>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" f64 ddot_(i32*, f64 *, i32*, f64 *, i32*);

const int N = 20;

void test_ddot(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    i32 incy = (rand() % 16) - 8;
    u32 dx = n*abs(incx); if (0 == dx) dx = 1;
    u32 dy = n*abs(incy); if (0 == dy) dy = 1;

    f64 *x = (f64*)CDC8600::memalloc(dx*2);
    f64 *y = (f64*)CDC8600::memalloc(dy*2);

    for (u32 i = 0; i < dx; i++) { x[i] = double(drand48()); }
    for (u32 i = 0; i < dy; i++) { y[i] = double(drand48()); }

    f64 ref_ = ddot_(&n, x, &incx, y, &incy);
    f64 new_ = CDC8600::BLAS::ddot(n, x, incx, y, incy);
    double epsilon = 1e-6;
    bool pass = true;

    if (abs(ref_ - new_) >
        ((abs(ref_) < abs(new_) ? abs(ref_) : abs(new_)) + epsilon) * epsilon)
    {
        pass = false;
    }

    std::cout << "ddot [" << std::setw(2) << count << "] (n = " << std::setw(3) << n << ", incx = " << std::setw(2) << incx << ", incy = " << std::setw(2) << incy << ", # of instr = " << std::setw(9) << PROC[0].instr_count << ") : ";
    if (pass)
        std::cout << "PASS" << std::endl;
    else
        std::cout << "FAIL" << std::endl;

    if (n < 10) dump(PROC[0].trace);
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_ddot(i);
    }
    return 0;
}
