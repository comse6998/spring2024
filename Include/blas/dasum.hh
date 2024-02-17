#ifndef _dasum_HH_
#define _dasum_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // incx must not be zero or negative!
        f64 dasum     (u64 n, f64 *dx, u64 incx);
        f64 dasum_cpp (u64 n, f64 *dx, u64 incx);
        f64 dasum_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif