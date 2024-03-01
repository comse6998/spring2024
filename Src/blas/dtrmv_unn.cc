#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_unn.hh>
#include <blas/ddot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_unn(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {
            Call(dtrmv_unn_cpp)(n, a, lda, x, incx);
        }

        void dtrmv_unn_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {
            if (n <= 0 || lda < n || incx == 0)
                return;

            for (u32 i = 0; i < n; i++) {
                u64 ix = (incx > 0 ? i : (-n + 1 + i)) * incx;
                x[ix] = ddot(n - i, x + (incx > 0 ? (incx * i) : 0), incx, a + (i * lda) + i, lda);
            }
        }
    } // namespace BLAS
} // namespace CDC8600