#ifndef _dger_HH_
#define _dger_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void dger(u64 m, u64 n, f64 alpha, f64* x, i64 incx, f64* y, i64 incy, f64* a, u64 lda);
        void dger_cpp(u64 m, u64 n, f64 alpha, f64* x, i64 incx, f64* y, i64 incy, f64* a, u64 lda);
	    //void dger_asm();

    } // namespace BLAS
}; // namespace CDC8600

#endif
