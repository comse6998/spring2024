#ifndef _dtrmv_lnn_HH_
#define _dtrmv_lnn_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_lnn    (u64 n, f64* a, u64 lda, f64* x, i64 incx);
        void dtrmv_lnn_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx);
        // void dtrmv_lnn_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif