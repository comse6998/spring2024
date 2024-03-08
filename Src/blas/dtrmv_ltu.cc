#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_ltu.hh>
#include <blas/ddot.hh>
#include <blas/dcopy.hh>

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

            /* Method 1: Using a temp vector tempx */

            f64 *tempx = (f64*)CDC8600::memalloc(n);

            // Copy x to tempx in parallel (only in the case of multi core)
            #pragma omp parallel
            {
                if (nump() > 1)
                    dcopy((n / nump()) + (me() < n % nump()),
                    (x + abs(incx) * me()),
                    abs(incx) * nump(),
                    tempx + me(),
                    nump());
            }
            
            #pragma omp parallel
            {
                // Cyclic partition
                for  (i64 i = me(); i < n - 1; i += nump()) {
                    i64 ix;
                    ix = (incx <= 0) ? (-n + 1 + i) * incx : (i * incx);
                    if (nump() > 1) {
                        x[ix] = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1,
                            (incx > 0) ? (tempx + i + 1) : tempx, (incx > 0) ? 1 : (-1));
                    } else {
                        x[ix] = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1,
                            (incx < 0) ? x : x + ix + incx, incx);
                    }
                }
                
                // block-wise partition
                /*
                for  (i64 i = (n-1) / nump() * me() ; i < ((me() == nump() - 1) ? (n - 1) : ((n - 1) / nump() * (me() + 1))); i += 1) {
                    i64 ix;
                    ix = (incx <= 0) ? (-n + 1 + i) * incx : (i * incx);
                    if (nump() > 1) {
                        x[ix] = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1,
                            (incx > 0) ? (tempx + i + 1) : tempx, (incx > 0) ? 1 : (-1));
                    } else {
                        x[ix] = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1,
                            (incx < 0) ? x : x + ix + incx, incx);
                    }
                }
                */
            }

            CDC8600::memfree(tempx, n);


            /* Method 2: Using omp barrier and a local variable s to */

            /*
            int num_p;

            // Because we're using openmp barrier, all the cores should be distributed to 
            // a same number of iteration, otherwise we'll be blocked waiting for some cpus
            // to finish a task that does not exist.
            #pragma omp parallel
            {
                num_p = nump();
                for  (i64 i = me(); i <  ((n - 1) / nump()) * nump(); i += nump()) {
                    i64 ix;
                    ix = (incx <= 0) ? (-n + 1 + i) * incx : (i * incx);
                    f64 s = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1,
                        (incx < 0) ? x : x + ix + incx, incx);
                    #pragma omp barrier
                        x[ix] = s;

                }
            }

            // Finish up the rest of the iterations that can't be parallelized.
            // This should be no more than (nump() - 1) iterations.
            for  (i64 i = ((n - 1) / num_p) * num_p; i < n - 1; i++) {
                i64 ix;
                ix = (incx <= 0) ? (-n + 1 + i) * incx : (i * incx);
                x[ix] = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1,
                            (incx < 0) ? x : x + ix + incx, incx);
            }
            */
        }
    } // namespace BLAS
} // namespace CDC8600
