#ifndef _zswap_HH_
#define _zswap_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        
        void zswap    (u64 n, c128 *x, i64 incx, c128 *y, i64 incy);
        void zswap_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy);
	void zswap_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
