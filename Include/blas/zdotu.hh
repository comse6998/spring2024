#ifndef _zdotu_HH_
#define _zdotu_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        c128 zdotu    (u64 n, c128 *zx, i64 incx, c128 *zy, i64 incy);
        c128 zdotu_cpp(u64 n, c128 *zx, i64 incx, c128 *zy, i64 incy);
	void zdotu_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
