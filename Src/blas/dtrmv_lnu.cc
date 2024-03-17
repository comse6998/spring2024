#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_lnu.hh>
#include <blas/ddot.hh>
#include <blas/dcopy.hh>

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

            u32 nx = n*abs(incx); if (0 == nx) nx = 1;
            f64 *Xtemp = (f64*)memalloc(nx);
        #pragma omp parallel
        {
            i32 size = n/nump() + (me() == nump()-1 ? n%nump() : 0);
            i32 offset = me() * (n/nump()) * incx;
            if(incx < 0)
            {
                if(me() == nump()-1) offset = 0;
                else offset = (-n)*incx + (me() +1)*(n/nump())*incx;
            }

            dcopy(size,X + offset,incx,Xtemp + offset,incx);
        }

        #pragma omp parallel
        {
            for(i32 i = n-1-me();i>0;i-=nump())
            {
                i64 ix = incx < 0 ? (i-n+1) * incx: i*incx;
                X[ix] = Xtemp[ix] +  ddot(i, Xtemp + (incx < 0 ? (i-n)*incx : 0), incx, A + i, lda);
            }
        }

        memfree(Xtemp,n);
        }
    }
}