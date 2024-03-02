#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_lnn.hh>
#include <blas/ddot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_lnn(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {
            Call(dtrmv_lnn_cpp)(n, a, lda, x, incx);
        }

        void dtrmv_lnn_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {
            if (n <= 0 || lda < n || incx == 0)
                return;

            i64 ix = incx>0? (n-1)*incx : 0;

            for (i64 j = n - 1; j >= 0; --j) {
                if (incx < 0) {
                    f64 temp = ddot(j + 1, a + j, lda, x-j*incx, incx);
                    x[ix] = temp;
                    ix -= incx;
                }

                else {
                    f64 temp = ddot(j + 1, a + j, lda, x, incx);
                    x[ix] = temp;
                    ix -= incx;
                }
            }
        }

        // Plain Cpp.

        // void dtrmv_lnn_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        // {
        //     if (n <= 0 || lda < n || incx == 0)
        //         return;

        //     i64 kx = incx >= 0? 0 : -(n - 1) * incx;
        //     for (i64 j = n - 1; j >= 0; --j) {
        //         i64 jx = kx + j * incx;
        //         if (x[jx] != 0) {
        //             f64 temp = x[jx];
        //             for (i64 i = n - 1; i > j; --i) {
        //                 i64 ix = kx + i * incx;
        //                 x[ix] += temp * a[i + j * lda];
        //             }
        //             x[jx] *= a[j + j * lda];
        //         }
        //     }
        // }
    } // namespace BLAS
} // namespace CDC8600