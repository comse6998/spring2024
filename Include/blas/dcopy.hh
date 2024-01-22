#ifndef _dcopy_HH_
#define _dcopy_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void dcopy    (u64 n, f64 *x, i64 incx, f64 *y, i64 incy);
        void dcopy_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy);
	void dcopy_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
