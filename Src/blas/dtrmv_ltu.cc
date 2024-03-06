#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_ltu.hh>
#include <blas/ddot.hh>
#include <blas/dcopy.hh>
// #include <cstring>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_ltu(i64 n, f64 *A, i64 lda, f64 *x, i64 incx)
        {
            Call(dtrmv_ltu_cpp)(n, A, lda, x, incx);
        }

        void dtrmv_ltu_cpp(i64 n, f64 *A, i64 lda, f64 *x, i64 incx)
        {
            // Input check
            if (n <= 0 || incx == 0 || lda <= 0 || lda < n)
                return;
            u64 nx = (n-1) * abs(incx) + 1;
            f64 *tempx = (f64*)CDC8600::memalloc(n);

            // memcpy(tempx, x, nx * sizeof(f64)); // Not good, not tracked.
            
            // dcopy(n, x, 1, tempx, 1);
            // dcopy(n, x, incx, tempx, 1);

            #pragma omp parallel
            {
                dcopy((n / nump()) + (me() < n % nump()),
                (x + abs(incx) * me()),
                abs (incx) * nump(),
                tempx + me(),
                nump());
            }
            

            // #pragma omp parallel
            // {
            //     dcopy()
            // }

            #pragma omp parallel
            {
                for  (i64 i = me(); i < n - 1; i += nump()) {
                    i64 ix;
                    ix = (incx <= 0) ? (-n + 1 + i) * incx : (i * incx);
                    x[ix] = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1, (incx > 0) ? (tempx + i + 1) : tempx, (incx > 0) ? 1 : (-1));
                }
            }
            
        }
    } // namespace BLAS
} // namespace CDC8600
