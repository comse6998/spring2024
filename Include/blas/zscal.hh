#ifndef _zscal_HH_
#define _zscal_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void zscal(i64 n, c128 a, c128 *x, i64 incx);
        void zscal_cpp(i64 n, c128 a, c128 *x, i64 incx);
        void zscal_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif