#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_ltn.hh>
#include <blas/ddot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_ltn(u64 n, f64* A, u64 lda, f64* X, i64 incx)
        {
            Call(dtrmv_ltn_cpp)(n, A, lda, X, incx);
        }

        void dtrmv_ltn_cpp(u64 n, f64* A, u64 lda, f64* X, i64 incx)
        {
            if(n == 0 || incx == 0 || lda < max((u64)1,n))
            {
                return;
            }

            i64 ix = 0;
            if (incx < 0) ix = (-n + 1) * incx;
            for(u32 i = 0; i < n ;i++)
            {
                // when i is 0, it seems like that x is in the bottom now
                X[ix] = ddot(n-i, X + (incx <0 ? 0 : i*incx), incx, A + i*lda + i, 1);
                ix += incx;
            }
        }
    }
}