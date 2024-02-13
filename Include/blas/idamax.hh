#ifndef _idamax_HH_
#define _idamax_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        i32 idamax    (u64 n, f64 *x, i64 incx);
        i32 idamax_cpp(u64 n, f64 *x, i64 incx);
	void idamax_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif