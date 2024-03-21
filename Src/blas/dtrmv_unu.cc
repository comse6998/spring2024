#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_unu.hh>
#include <blas/ddot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_unu(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {
            Call(dtrmv_unu_cpp)(n, a, lda, x, incx);
        }

        void dtrmv_unu_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {

            
            if (n < 0 || lda < n || lda < 1 || incx == 0)
                return;

            u64 syn = 0;
#pragma omp parallel
            {
                for (u64 i = me(); i < n; i+=nump()){
                    u64 ix = (incx > 0 ? i : (-n + 1 + i)) * incx;
                    f64 tmp = x[ix] + ddot(n-i-1, x+(i+1)*(incx>0 ? incx:0), incx, a+(i+1)*lda+i, lda);
                    while(syn < i);
                    x[ix] = tmp;
                    syn++;
                }
            }
            return;
        }
    } // namespace BLAS
} // namespace CDC8600