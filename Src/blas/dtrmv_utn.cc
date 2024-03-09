#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_utn.hh>
#include <blas/ddot.hh>
#include <blas/dcopy.hh>

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
            f64 *tmpX = (f64*)CDC8600::memalloc(n*abs(incx));    // Input matrix
            if (n <= 0 || incx == 0 || lda < n)  return;
            i64 ix;
            #pragma omp parallel private (ix)
            {
                // CYCLIC PARTITION
                // for(i64 i = n - me(); i > 0; i = i - nump())
                // {
                //     if(incx > 0)
                //     {
                //         ix = i*incx;
                //     }
                //     else
                //     {
                //         ix = (-n+i) * (incx);
                //     }
                //     if(incx > 0)
                //     {
                //         tmpX[ix-incx] = ddot(i, x, incx, a + ((i-1) * (lda)), 1);
                //     }
                //     else
                //     {
                //         tmpX[ix] = ddot(i, x + ((-n+i)*incx), incx, a + ((i-1) * (lda)), 1);
                //     }
                // }

                //BLOCKING PARTITION
                i64 limit;
                if (me() == nump() - 1)
                    limit = 0;
                else
                    limit = n - ((me() + 1)*(n-1)/nump());
                for(i64 i = n - me()*(n-1)/nump(); i > limit; i--)
                {
                    if(incx > 0)
                    {
                        ix = i*incx;
                    }
                    else
                    {
                        ix = (-n+i) * (incx);
                    }
                    if(incx > 0)
                    {
                        tmpX[ix-incx] = ddot(i, x, incx, a + ((i-1) * (lda)), 1);
                    }
                    else
                    {
                        tmpX[ix] = ddot(i, x + ((-n+i)*incx), incx, a + ((i-1) * (lda)), 1);
                    }
                }
            }            
            #pragma omp parallel
            {
                for(u64 i= me(); i < n; i += nump())
                {
                    dcopy(1, tmpX + (i*abs(incx)), incx, x + (i*abs(incx)), incx);
                }
            }
        }
    } // namespace BLAS
} // namespace CDC8600
