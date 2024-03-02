#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_utn.hh>
#include <blas/ddot.hh>

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
            if (n <= 0 || incx == 0 || lda <= 0 || lda < n)  return;
            i64 ix = 0;
            for (i64 i = n-1; i >= 0; i--) {
                x[ix] = x[ix] + ddot((i), a + i, lda*i, x + ix,  incx);
                cout << "n= " << (i) << ", dx= " << (i);
                cout << ", incx= " << 1 << ", dy= " << (lda * i ) << ", incy= " << 1 << " ix = "<<  ix << endl;
                ix += incx;
            }
        }
    } // namespace BLAS
} // namespace CDC8600
