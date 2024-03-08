#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dgemv_td.hh>
#include <blas/ddot.hh>
#include <blas/dscal.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dgemv_td    (u64 m, u64 n, f64 alpha, f64* a, u64 lda, f64* x, i64
                incx, f64 beta, f64 *y, i64 incy)
        {
            Call(dgemv_td_cpp)(m, n, alpha, a, lda, x, incx, beta, y, incy);
        }

        void dgemv_td_cpp(u64 m, u64 n, f64 alpha, f64* a, u64 lda, f64* x, i64
                incx, f64 beta, f64 *y, i64 incy)
        {
            if (m <= 0 || n <= 0 || lda < m || incx == 0 || incy == 0)
                return;

            dscal(n, beta, y, abs(incy));

            i64 iy = 0;
            if (incy <= 0) iy = (-n + 1) * incy;

#pragma omp parallel
            for (u64 i = me(); i < n; i += nump())
                y[iy + incy*i] += alpha * ddot(m, x, incx, a + i*lda, 1);
        }
    } // namespace BLAS
} // namespace CDC8600
