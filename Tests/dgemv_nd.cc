#include <blas/dgemv_nd.hh>  // Make sure to include the correct header file
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" i32 dgemv_(char *, i32 *, i32 *, f64 *, f64 *, i32 *, f64 *, i32 *, f64 *, f64 *, i32 *);

const int N = 20;
const double epsilon = std::pow(10, -6);

void test_dgemv_nd(int count) {
    reset();

    i32 m = rand() % 256;
    i32 n = rand() % 256;
    i32 lda = m + rand() % 256;

    i32 incx = (rand() % 16) - 8; if (incx == 0) incx = 1;
    u32 nx = n*abs(incx);
    if (0 == nx) nx = 1;

    i32 incy = (rand() % 16) - 8; if (incy == 0) incy = 1;
    u32 ny = m*abs(incy);
    if (0 == ny) ny = 1;

    f64 alpha = drand48();
    f64 beta = drand48();

    f64 *a = (f64*)CDC8600::memalloc(lda*n);
    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *y = (f64*)CDC8600::memalloc(ny);
    f64 *y_ = (f64*)CDC8600::memalloc(ny);

    for (i32 i = 0; i < lda*n; i++) { a[i] = drand48(); }
    for (u32 i = 0; i < nx; i++) {    x[i] = drand48(); }
    for (u32 i = 0; i < ny; i++) {    y_[i] = y[i] = drand48(); }

    dgemv_((char *) "n", &m, &n, &alpha, a, &lda, x, &incx, &beta, y, &incy);

    CDC8600::BLAS::dgemv_nd(m, n, alpha, a, lda, x, incx, beta, y_, incy);

    bool pass = true;
    for (u32 i = 0; i < ny; i++)
    {
        if (abs(y[i] - y_[i]) > (min(abs(y[i]), abs(y_[i])) + epsilon) * epsilon)
	{
            pass = false;
            //std::cerr << "Mismatch at " << i << ": got " << y_[i] << ", expected " << y[i] << std::endl;
        }

    }

    cout << "dgemv_nd [" << setw(2) << count << "] ";
    cout << "(m = " << setw(3) << m;
    cout << ", n = " << setw(3) << n;
    cout << ", alpha = " << setw(5) << alpha;
    cout << ", lda = " << setw(2) << lda;
    cout << ", incx = " << setw(2) << incx;
    cout << ", beta = " << setw(5) << beta;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";

    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
}

int main() {
    for (int i = 0; i < N; i++) {
        test_dgemv_nd(i);
    }
    return 0;
}
