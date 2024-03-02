#ifndef _dtrmv_unu_HH_
#define _dtrmv_unu_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_unu    (u64 n, f64* a, u64 lda, f64* x, i64 incx);
        void dtrmv_unu_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx);
    } // namespace BLAS
}; // namespace CDC8600

#endif