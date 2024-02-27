#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_lnu.hh>
#include <blas/ddot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_lnu(u64 n, f64* A, u64 lda, f64* X, i64 incx)
        {
            Call(dtrmv_lnu_cpp)(n, A, lda, X, incx);
        }

        void dtrmv_lnu_cpp(u64 n, f64* A, u64 lda, f64* X, i64 incx)
        {
            if(n == 0 || incx == 0 || lda < max((u64)1,n))
            {
                return;
            }

            i64 ix = (n-1)*incx;
            if(incx < 0)
            {
                ix = 0;
            }
            for(u32 i = n-1;i>0;i--)
            {
                X[ix] = X[ix] +  ddot(i, X, incx, A + i, lda);
                ix -= incx;
            }
        }
    }
}