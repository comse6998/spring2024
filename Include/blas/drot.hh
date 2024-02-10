#ifndef _drot_HH_
#define _drot_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void drot    (u64 n, f64 *x, i64 incx, f64 *y, i64 incy, f64 c, f64 s);
        void drot_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy, f64 c, f64 s);
	void drot_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
