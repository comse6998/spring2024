#ifndef _dgemv_td_HH_
#define _dgemv_td_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dgemv_td    (u64 m, u64 n, f64 alpha, f64* a, u64 lda, f64* x, i64
                incx, f64 beta, f64 *y, i64 incy);
        void dgemv_td_cpp(u64 m, u64 n, f64 alpha, f64* a, u64 lda, f64* x, i64
                incx, f64 beta, f64 *y, i64 incy);
    } // namespace BLAS
}; // namespace CDC8600

#endif
