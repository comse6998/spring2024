#ifndef _dgemv_ta_HH_
#define _dgemv_ta_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        
        void dgemv_ta    (u64 m, u64 n, f64 alpha, f64 *A, u64 LDA, f64 *x, i64 incx, f64 beta, f64 *y, i64 incy);
        void dgemv_ta_cpp(u64 m, u64 n, f64 alpha, f64 *A, u64 LDA, f64 *x, i64 incx, f64 beta, f64 *y, i64 incy);
	void dgemv_ta_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
