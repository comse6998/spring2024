#ifndef _dswap_HH_
#define _dswap_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void dswap    (u64 n, f64 *x, i64 incx, f64 *y, i64 incy);
        void dswap_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy);
	void dswap_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
