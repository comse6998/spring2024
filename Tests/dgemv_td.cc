#include <blas/dgemv_td.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" i32 dgemv_(char *, i32 *, i32 *, f64 *, f64 *, i32 *, f64 *, i32 *,
        f64 *, f64 *, i32 *);

const int N = 20;
const double epsilon = 1e-9;

void test_dgemv_td(int count, bool traceon, i32 m, i32 n, i32 lda, i32 incx, i32 incy)
{
    reset();

    u32 nx = m*abs(incx); if (0 == nx) nx = 1;

    u32 ny = n*abs(incy); if (0 == ny) ny = 1;

    f64 alpha = drand48();
    f64 beta = drand48();

    tracing = false;

    f64 *a = (f64*)CDC8600::memalloc(lda*n);	// Output matrix
    f64 *x = (f64*)CDC8600::memalloc(nx);	// Output matrix
    f64 *y = (f64*)CDC8600::memalloc(ny);	// Output matrix
    f64 *y_test = (f64*)CDC8600::memalloc(ny);	// Output matrix

    for (i32 i = 0; i < lda*n; i++) { a[i] = drand48(); }
    for (u32 i = 0; i < nx; i++) { x[i] = drand48(); }
    for (u32 i = 0; i < ny; i++) { y_test[i] = y[i] = drand48(); }

    dgemv_((char *) "T", &m, &n, &alpha, a, &lda, x, &incx, &beta, y, &incy);

    CDC8600::BLAS::dgemv_td(m, n, alpha, a, lda, x, incx, beta, y_test, incy);

    bool pass = true;
    for (u32 i = 0; i < ny; ++i)
    {
    if (abs(y[i] - y_test[i]) > ((abs(y[i]) < abs(y_test[i]) ? abs(y[i]) :
                    abs(y_test[i])) + epsilon) * epsilon)
            pass = false;
    }

    cout << fixed;
    cout.precision(5);

    cout << "dgemv_td [" << setw(2) << count << "] ";
    cout << "(m = " << setw(3) << m;
    cout << ", n = " << setw(3) << n;
    cout << ", alpha = " << alpha;
    cout << ", lda = " << setw(3) << lda;
    cout << ", incx = " << setw(2) << incx;
    cout << ", beta = " << beta;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
    cout << ", # of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
    cout << ") : ";

    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (traceon) {
        for (u32 p = 0; p < params::Proc::N; ++p) {
            dump(PROC[p].trace, (char *) ("dgemv_td.tr." + to_string(p)).c_str());
        }
    }
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
	for (u32 i = 0; i < N; i++)
	{
            i32 m = rand() % 256;
            i32 n = rand() % 256;
            i32 lda = m + rand() % 256;
            i32 incx = (rand() % 16) - 8; if (incx == 0) incx = 1;
            i32 incy = (rand() % 16) - 8; if (incy == 0) incy = 1;
	    test_dgemv_td(i, false, m, n, lda, incx, incy);
	}
    }
    else if (argc == 6)
    {
        i32 m = atoi(argv[1]);
	i32 n = atoi(argv[2]);
        i32 lda = atoi(argv[3]);
	i32 incx = atoi(argv[4]);
	i32 incy = atoi(argv[5]);
        if (lda < m) {
            cerr << "lda must be larger than m" << endl;
            exit(1);
        }
        test_dgemv_td(0, true, m, n, lda, incx, incy);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [m n lda incx incy]" << endl;
	exit(1);
    }
}
