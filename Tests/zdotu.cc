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

void test_zdotu(int count, bool traceon, i32 n, i32 incx, i32 incy)
{
    reset();

    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 ny = n*abs(incy); if (0 == ny) ny = 1;

    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *y = (c128*)CDC8600::memalloc(ny*2);

    for (int i = 0; i < nx; i++) { x[i] = c128(drand48(), drand48()); }
    for (int i = 0; i < ny; i++) { y[i] = c128(drand48(), drand48()); }

    tracing = false;

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

    if (traceon) dump(PROC[0].trace, "zdotu.tr");
}
    

int main(int argc, char **argv)
{
    if (argc == 1)
    {
	for (u32 i = 0; i < N; i++)
	{
	    i32 n = rand() % 256;
	    i32 incx = (rand() % 16) - 8;
	    i32 incy = (rand() % 16) - 8;
	    test_zdotu(i, false, n, incx, incy);
	}
    }
    else if (argc == 4)
    {
	i32 n = atoi(argv[1]);
	i32 incx = atoi(argv[2]);
	i32 incy = atoi(argv[3]);
        test_zdotu(0, true, n, incx, incy);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [n incx incy]" << endl;
	exit(1);
    }
}
