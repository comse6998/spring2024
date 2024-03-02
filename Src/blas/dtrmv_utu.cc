#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_utu.hh>
#include <blas/ddot.hh>


// DTRMV_UTU (upper/transpose/unit)
// A -> upper triangle of A
// A -> diagonal = 1
// x := A**T*x

namespace CDC8600
{
    namespace BLAS
    {
        void dtrmv_utu(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {
            Call(dtrmv_utu_cpp)(n, a, lda, x, incx);
        }
        void dtrmv_unu_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        {
            if (n < 0 || lda < n || lda < 1 || incx == 0)
                return;

            for (u64 i = 0; i < n; i++){
                u64 ix = (incx > 0 ? i : (-n + 1 + i)) * incx;
                //x[ix] = x[ix] + ddot(n-i-1, x+(i+1)*(incx>0 ? incx:0), incx, a+(i+1)*lda+i, lda);
                //x[ix] = x[ix] + ddot(n-i-1, a+(i+1)*lda+i, incx, x+(i+1)*(incx>0 ? incx:0), lda);
                //x[ix] = x[ix] + ddot(n-i-1, x+(i+1)*(incx>0 ? incx:0), incx, a+ lda * i + i + 1, lda);
                //x[ix] = x[ix] + ddot(n-i-1, x+(i+1)*(incx>0 ? incx:0), incx, a+(lda+1)*i+lda, lda); 
                //x[ix] = x[ix] + ddot(n-i-1, x+(i+1)*(incx>0 ? incx:0), lda, a+(i+1)*lda+i, incx);
                x[ix] = x[ix] + ddot(n-i-1, x+(i+1)*(incx>0 ? incx:0), incx, a+(i+1)*lda+i, lda);
            }
            return;
        }
        void dtrmv_utu_cpp(u64 n, f64* A, u64 lda, f64* x, i64 incx)
        {
            // Input check
            if (n <= 0 || incx == 0 || lda <= 0 || lda < n)
                return;
            i64 ix = (incx <= 0) ? (-n + 1) * incx : 0;
            for (i64 i = n; i > 0; i--) {
                x[ix] = x[ix] + ddot(n - i - 1,  A + lda * i + i + 1, 1, (incx < 0) ? x : x + ix + incx, incx);
                ix -= incx;
        }
    }

     // namespace BLAS
} // namespace CDC8600
