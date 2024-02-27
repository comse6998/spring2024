#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_ltu.hh>
#include <blas/dcopy.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_ltu(u64 n, f64 *A, u64 lda, f64 *x, i64 incx)
        {
            Call(dtrmv_ltu_cpp)(n, A, lda, x, incx);
        }

        void dtrmv_ltu_cpp(u64 n, f64 *A, u64 lda, f64 *x, i64 incx)
        {
            // Input check
            return;
        }
    } // namespace BLAS
} // namespace CDC8600
