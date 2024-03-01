#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_ltu.hh>
#include <blas/dcopy.hh>

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
            return;
        }
    } // namespace BLAS
} // namespace CDC8600
