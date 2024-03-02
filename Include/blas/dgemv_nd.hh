#ifndef _DGEMV_HH_
#define _DGEMV_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dgemv_nd(u64 m, u64 n, f64 alpha, f64* a, u64 lda, f64* x, i64 incx, f64 beta, f64 *y, i64 incy);
        void dgemv_nd_cpp(u64 m, u64 n, f64 alpha, f64* a, u64 lda, f64* x, i64 incx, f64 beta, f64 *y, i64 incy);
    } // namespace BLAS
};    // namespace CDC8600

#endif
