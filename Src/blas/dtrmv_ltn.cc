#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_ltn.hh>
#include <blas/ddot.hh>
#include <blas/dcopy.hh>
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
            f64 *Y = (f64*)CDC8600::memalloc(n);
            i64 incy = incx;
        #pragma omp parallel // make the following work in parallel
        {
            if (nump() > 1){
                incy = incx/abs(incx);
                dcopy(n / nump() + (me() < n % nump() ? 1 : 0), 
                        X + me() * abs(incx), 
                        incx * nump(), 
                        Y + me(), 
                        incy * nump());
            }
        }
        #pragma omp parallel
        {
            
            for(u64 i = me(); i < n ;i+=nump())
            {
                u64 ix;
                ix = (incx < 0) ? (-n + 1 + i) * incx : i*incx;
                // when i is 0, it seems like that x is in the bottom now
                if (nump() > 1)
                {
                    X[ix] = ddot(n-i, Y + (incy <0 ? 0 : i*incy), incy, A + i*lda + i, 1);
                }
                else{
                X[ix] = ddot(n-i, X + (incx <0 ? 0 : i*incx), incx, A + i*lda + i, 1);
                }
            }

        }

        CDC8600::memfree(Y, n);
        }
    
    
  
    }
}