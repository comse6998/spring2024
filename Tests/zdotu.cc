#include <blas/zdotu.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" c128 zdotu_(i32*, c128*, i32*, c128*, i32*);

const int N = 20;
const double epsilon = 1e-9;

void test_zdotu(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    i32 incy = (rand() % 16) - 8;
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 ny = n*abs(incy); if (0 == ny) ny = 1;

    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *y = (c128*)CDC8600::memalloc(ny*2);

    for (int i = 0; i < nx; i++) { x[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < ny; i++) { y[i] = c128(drand48(), drand48()); }

    tracing = false; if (n < 10) tracing = true;

    c128 res_ref = zdotu_(&n, x, &incx, y, &incy);
    c128 res_new = CDC8600::BLAS::zdotu(n, x, incx, y, incy);

    bool pass = true;

    // Have to check equality of both real and imag parts separately :/
    double real_ref = res_ref.real();
    double real_new = res_new.real();
    double imag_ref = res_ref.imag();
    double imag_new = res_new.imag();
    if ((abs(real_ref - real_new) >
          ((abs(real_ref) < abs(real_new) ? abs(real_ref) : abs(real_new)) +
              epsilon) *
         epsilon) ||
        (abs(imag_ref - imag_new) >
          ((abs(imag_ref) < abs(imag_new) ? abs(imag_ref) : abs(imag_new)) +
              epsilon) *
         epsilon))
    {
      pass = false;
    }

    cout << "zdotu [" << setw(2) << count << "] ";
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (n < 10) dump(PROC[0].trace, "zdotu.tr");
}
    

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_zdotu(i);
    }
    return 0;
}
