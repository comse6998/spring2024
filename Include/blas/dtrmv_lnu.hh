#ifndef _dtrmv_lnu_HH_
#define _dtrmv_lnu_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_lnu    (u64 n, f64* A, u64 lda, f64* X, i64 incx);
        void dtrmv_lnu_cpp(u64 n, f64* A, u64 lda, f64* X, i64 incx);
    } // namespace BLAS
}; // namespace CDC8600

#endif
