#ifndef _dtrmv_ltu_HH_
#define _dtrmv_ltu_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_ltu    (i64 n, f64 *A, i64 lda, f64 *x, i64 incx);
        void dtrmv_ltu_cpp(i64 n, f64 *A, i64 lda, f64 *x, i64 incx);
        // void dtrmv_ltu_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif