#ifndef _dtrmv_utn_HH_
#define _dtrmv_utn_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_utn    (u64 n, f64* a, u64 lda, f64* x, i64 incx);
        void dtrmv_utn_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx);
    }
     // namespace BLAS
}; // namespace CDC8600

#endif
