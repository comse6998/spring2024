#ifndef _daxpy_HH_
#define _daxpy_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void daxpy    (u64 n, f64 a, f64 *x, i64 incx, f64 *y, i64 incy);
        void daxpy_cpp(u64 n, f64 a, f64 *x, i64 incx, f64 *y, i64 incy);
	    void daxpy_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
