#ifndef _dtrmv_utu_HH_
#define _dtrmv_utu_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_utu    (u64 n, f64* a, u64 lda, f64* x, i64 incx);
        void dtrmv_utu_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx);
        // void dtrmv_unn_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif