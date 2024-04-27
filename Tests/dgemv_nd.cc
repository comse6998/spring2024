#include <blas/dgemv_nd.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <omp.h>

using namespace CDC8600;

extern "C" i32 dgemv_(char *, i32 *, i32 *, f64 *, f64 *, i32 *, f64 *, i32 *, f64 *, f64 *, i32 *);

const int N = 20;
const double epsilon = std::pow(10, -6);

void test_dgemv_nd(int count, bool traceon, int m, int n, int incx, int incy, double alpha, double beta) {
    reset();

    i32 lda = m + rand() % 256;

    u32 nx = n * abs(incx);
    if (0 == nx) nx = 1;

    u32 ny = m * abs(incy);
    if (0 == ny) ny = 1;

    f64 *a = (f64*)CDC8600::memalloc(lda * n);
    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *y = (f64*)CDC8600::memalloc(ny);
    f64 *y_ = (f64*)CDC8600::memalloc(ny);

    for (i32 i = 0; i < lda * n; i++) { a[i] = drand48(); }
    for (u32 i = 0; i < nx; i++) { x[i] = drand48(); }
    for (u32 i = 0; i < ny; i++) { y_[i] = y[i] = drand48(); }

    dgemv_((char *)"n", &m, &n, &alpha, a, &lda, x, &incx, &beta, y, &incy);

    CDC8600::BLAS::dgemv_nd(m, n, alpha, a, lda, x, incx, beta, y_, incy);

    bool pass = true;
    for (u32 i = 0; i < ny; i++) {
        if (abs(y[i] - y_[i]) > (min(abs(y[i]), abs(y_[i])) + epsilon) * epsilon) {
            pass = false;
        }
    }
    cout << "dgemv_nd [" << setw(2) << count << "] ";
    cout << "(m = " << setw(3) << m;
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", incy = " << setw(3) << incy;
    cout << ", alpha = " << setw(3) << alpha;
    cout << ", beta = " << setw(3) << beta;
    cout << ", # of instr  = ("; cout << setw(9) << PROC[0].instr_count; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].instr_count; cout << ")";
    cout << ", # of ops    = ("; cout << setw(9) << PROC[0].op_count   ; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].op_count   ; cout << ")";
    cout << ", # of cycles = ("; cout << setw(9) << PROC[0].op_maxcycle; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].op_maxcycle; cout << ")";
    cout << ") : ";
    if (pass)
        cout << ") PASS" << endl;
    else
        cout << ") FAIL" << endl;

    CDC8600::memfree(a, lda * n);
    CDC8600::memfree(x, nx);
    CDC8600::memfree(y, ny);
    CDC8600::memfree(y_, ny);

    if (traceon) dump(PROC[0].trace, "dgemv_nd.tr.0");
    if (traceon) dump(PROC[1].trace, "dgemv_nd.tr.1");
    if (traceon) dump(PROC[2].trace, "dgemv_nd.tr.2");
    if (traceon) dump(PROC[3].trace, "dgemv_nd.tr.3");

}

int main(int argc, char **argv) {
    if (1 == argc)
    {
        for (u32 i = 0; i < N; i++)
        {
            i32 m = rand() % 256;
            i32 n = rand() % 256;
            i32 incx = (rand() % 16) - 8; if (incx == 0) incx = 1;
            i32 incy = (rand() % 16) - 8; if (incy == 0) incy = 1;
            f64 alpha = drand48();
            f64 beta = drand48();
            test_dgemv_nd(i + 1, false, m, n, incx, incy, alpha, beta);
        }
    } else if (argc == 7) {
        int m = atoi(argv[1]);
        int n = atoi(argv[2]);
        int incx = atoi(argv[3]);
        int incy = atoi(argv[4]);
        double alpha = atof(argv[5]);
        double beta = atof(argv[6]);

        test_dgemv_nd(0, true, m, n, incx, incy, alpha, beta);
    } else {
        std::cerr << "Usage: " << argv[0] << " m n incx incy alpha beta" << std::endl;
        return -1;
    }
    return 0;
}
