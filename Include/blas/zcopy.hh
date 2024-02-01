#ifndef _zcopy_HH_
#define _zcopy_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void zcopy    (u64 n, c128 *x, i64 incx, c128 *y, i64 incy);
        void zcopy_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy);
	void zcopy_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
