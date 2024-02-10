#ifndef _zaxpy_HH_
#define _zaxpy_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void zaxpy    (u64 n, c128 a, c128 *x, i64 incx, c128 *y, i64 incy);
        void zaxpy_cpp(u64 n, c128 a, c128 *x, i64 incx, c128 *y, i64 incy);
	    void zaxpy_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
