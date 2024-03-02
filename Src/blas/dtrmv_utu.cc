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
