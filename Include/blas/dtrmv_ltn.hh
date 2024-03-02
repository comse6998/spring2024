#ifndef _dtrmv_ltn_HH_
#define _dtrmv_ltn_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_ltn    (u64 n, f64* A, u64 lda, f64* X, i64 incx);
        void dtrmv_ltn_cpp(u64 n, f64* A, u64 lda, f64* X, i64 incx);
    } // namespace BLAS
}; // namespace CDC8600

#endif
