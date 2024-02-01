#ifndef _dscal_HH_
#define _dscal_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void dscal    (u64 n, f64 *x, i64 incx, f64 *y, i64 incy);
        void dscal_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy);
	void dscal_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
