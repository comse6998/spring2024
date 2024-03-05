#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_utn.hh>
#include <blas/ddot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_utn(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {
            Call(dtrmv_utn_cpp)(n, a, lda, x, incx);
        }

        void dtrmv_utn_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        
        {
            if (n <= 0 || incx == 0 || lda < n)  return;
            for (i32 i = n; i > 0; i--) {
                i64 ix = (incx > 0 ? i : (-n+i)) * (incx);
                if(incx > 0)
                {
                x[ix-incx] = ddot(i, x, incx, a + ((i-1) * (lda)), 1);
                }
                else
                {
                x[ix] = ddot(i, x + ((-n+i)*incx), incx, a + ((i-1) * (lda)), 1);
                }
            }
        }
    } // namespace BLAS
} // namespace CDC8600
