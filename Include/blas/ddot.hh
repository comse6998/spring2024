#ifndef _ddot_HH_
#define _ddot_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        f64 ddot(u64 n, f64 *dx, i64 incx, f64 *dy, i64 incy);
        f64 ddot_cpp(u64 n, f64 *dx, i64 incx, f64 *dy, i64 incy);
	    void ddot_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif