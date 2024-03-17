#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_unn.hh>
#include <blas/ddot.hh>
#include <blas/dcopy.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_unn(u64 n, f64* A, u64 lda, f64* X, i64 incx)
        {
            Call(dtrmv_unn_cpp)(n, A, lda, X, incx);
        }

        void dtrmv_unn_cpp(u64 n, f64* A, u64 lda, f64* X, i64 incx)
        {
            if (n <= 0 || lda < n || incx == 0)
                return;

            f64* tmp = (f64*)CDC8600::memalloc(n);
            f64* Y = X;
            i64 incy = incx;

#pragma omp parallel 
            {
                if (nump() > 1) {
                    Y = tmp;
                    incy = incx / abs(incx);

                    dcopy(n / nump() + (me() < n % nump() ? 1 : 0), 
                        X + me() * abs(incx), 
                        incx * nump(), 
                        Y + me(), 
                        incy * nump());
                } 
 
#pragma omp barrier  // make sure all copy finished

                for (u32 i = me(); i < n; i += nump()) {
                    u64 ix = (incx > 0 ? i : (-n + 1 + i)) * incx;
                    X[ix] = ddot(n - i, Y + (incy > 0 ? incy * i : 0), incy, A + i * lda + i, lda);
                }
            }

            CDC8600::memfree(tmp, n);
        }
    } // namespace BLAS
} // namespace CDC8600