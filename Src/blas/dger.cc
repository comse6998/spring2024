#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dger.hh>
#include <blas/daxpy.hh>

namespace CDC8600
{

    extern "C" void xerbla_(char*, int*);
    extern "C" i32 daxpy_(int32_t *, double *, double *, int32_t *, double *, int32_t *);


    namespace BLAS
    {
        void dger(u64 m, u64 n, f64 alpha, f64* x, i64 incx, f64* y, i64 incy, f64* a, u64 lda)
        {
            //Call(dger_cpp)(m, n, alpha, x, incx, y, incy, a, lda);
            dger_cpp(m, n, alpha, x, incx, y, incy, a, lda);
        }

/* void dger_cpp(u64 m, u64 n, f64 alpha, f64* x, i64 incx, f64* y, i64 incy, f64* a, u64 lda) {


    i32 M = m;
    i32 N = n;
    i32 INCX = incx;
    i32 INCY = incy;
    i32 one = 1;

    if (m <= 0 || n <= 0 || alpha == 0.0)
        return;

    if (incy > 0) {
        int jy = 0;
        for (int j = 0; j < n; ++j) {
            if (y[jy] != 0.0)
            {
                double temp = alpha * y[jy];
                daxpy(M, temp, x, INCX, &a[j * lda], one);
            }
            jy += incy;
        }
    } else {
        int jy = (1 - n) * incy;
        for (int j = 0; j < n; ++j) {
            if (y[jy] != 0.0)
            {
                double temp = alpha * y[jy];
                daxpy(M, temp, x, INCX, &a[j * lda], one);
            }
            jy += incy;
        }
    }
}  */

void dger_cpp(u64 m, u64 n, f64 alpha, f64* x, i64 incx, f64* y, i64 incy, f64* a, u64 lda) {


    i32 M = m;
    i32 N = n;
    i32 INCX = incx;
    i32 INCY = incy;
    i32 one = 1;

    if (m <= 0 || n <= 0 || alpha == 0.0)
        return;

#pragma omp parallel

    if (incy > 0) {
        int jy = 0;
        for (int j = me(); j < n; j += nump()) {
            if (y[jy] != 0.0)
            {
                double temp = alpha * y[jy];
                daxpy(M, temp, x, INCX, &a[j * lda], one);
            }
            jy += incy;
        }
    } else {
        int jy = (1 - n) * incy;
        for (int j = me(); j < n; j += nump()) {
            if (y[jy] != 0.0)
            {
                double temp = alpha * y[jy];
                daxpy(M, temp, x, INCX, &a[j * lda], one);
            }
            jy += incy;
        }
    }
} 
      // Local Variables
 /*    double temp;
    int i, info, ix, j, jy, kx;
    const double zero = 0.0;

    // Test the input parameters
    info = 0;
    if (m < 0)
        info = 1;
    else if (n < 0)
        info = 2;
    else if (incx == 0)
        info = 5;
    else if (incy == 0)
        info = 7;
    else if (lda < std::max(1, int(m)))
        info = 9;

    if (info != 0) {
        char srname[] = "dger  ";
        xerbla_(srname, &info);
        return;
    }

    // Quick return if possible
    if (m == 0 || n == 0 || alpha == zero)
        return;

    // Start the operations
    if (incy > 0)
        jy = 1;
    else
        jy = 1 - (n - 1) * incy;

    if (incx == 1) {
        for (j = 0; j < n; ++j) {
            if (y[jy - 1] != zero) {
                temp = alpha * y[jy - 1];
                for (i = 0; i < m; ++i)
                    a[i + j * lda] += x[i] * temp;
            }
            jy += incy;
        }
    } else {
        if (incx > 0)
            kx = 1;
        else
            kx = 1 - (m - 1) * incx;

            //kx = (-m + 1) * incx;


        for (j = 0; j < n; ++j) {
            if (y[jy - 1] != zero) {
                temp = alpha * y[jy - 1];
                ix = kx;
                for (i = 0; i < m; ++i) {
                    a[i + j * lda] += x[ix - 1] * temp;
                    ix += incx;
                }
            }
            jy += incy;
        }
    }  */

    } // namespace BLAS
} // namespace CDC8600
