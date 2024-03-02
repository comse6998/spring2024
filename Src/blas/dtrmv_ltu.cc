#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_ltu.hh>
#include <blas/ddot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_ltu(i64 n, f64 *A, i64 lda, f64 *x, i64 incx)
        {
            Call(dtrmv_ltu_cpp)(n, A, lda, x, incx);
        }

        void dtrmv_ltu_cpp(i64 n, f64 *A, i64 lda, f64 *x, i64 incx)
        {
            // Input check
            if (n <= 0 || incx == 0 || lda <= 0 || lda < n)
                return;
            i64 ix = (incx <= 0) ? (-n + 1) * incx : 0;
            for (i64 i = 0; i < n; i++) {
                x[ix] = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1, (incx < 0) ? x : x + ix + incx, incx);
                ix += incx;
            }
        }
    } // namespace BLAS
} // namespace CDC8600
