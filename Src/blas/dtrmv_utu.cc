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
        
        void dtrmv_utu_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx)
        
        {
            if (n <= 0 || incx == 0 || lda < n)  return;
            i64 ix; 
            for (i32 i = n; i > 0; i--) {
                if(incx > 0){
                ix = i * incx;
                x[ix-incx] = x[ix - incx] + ddot(i-1, x, incx, a + ((i-1) * (lda)), 1);
                }
                else{
                ix = (-n+i) * (incx);
                x[ix] = x[ix] + ddot(i-1, x + ((-n+i-1)*incx), incx, a + ((i-1) * (lda)), 1);
                }
            }
        }
    }
  // namespace BLAS
} // namespace CDC8600
