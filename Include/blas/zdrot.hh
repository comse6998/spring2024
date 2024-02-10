#ifndef _zdrot_HH_
#define _zdrot_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void zdrot    (u64 n, c128 *x, i64 incx, c128 *y, i64 incy, f64 c, f64 s);
        void zdrot_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy, f64 c, f64 s);
	    void zdrot_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
