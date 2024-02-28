#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dger.hh>
#include <blas/daxpy.hh>

namespace CDC8600
{

    extern "C" void xerbla_(char*, int*);
    extern "C" i32 daxpy_(int32_t *, double *, double *, int32_t *, double *, int32_t *);


    namespace BLAS
    {
        void dger(u64 m, u64 n, f64 alpha, f64* x, i64 incx, f64* y, i64 incy, f64* a, u64 lda)
        {
            //Call(dger_cpp)(m, n, alpha, x, incx, y, incy, a, lda);
            dger_cpp(m, n, alpha, x, incx, y, incy, a, lda);
        }
        
void dger_cpp(u64 m, u64 n, f64 alpha, f64* x, i64 incx, f64* y, i64 incy, f64* a, u64 lda) {

    if (m <= 0 || n <= 0 || alpha == 0.0)
        return;

#pragma omp parallel
{
    if (incy > 0) {
        int jy = me() * incy;

        for (int j = me(); j < n; j += nump()) {
            if (y[jy] != 0.0)
            {
                double temp = alpha * y[jy];
                daxpy((i32)m, temp, x, (i32)incx, &a[j * lda], 1);
            }
            jy += incy*nump();
        }
    } else {
        int jy = (1 - n) * incy + me() * incy;
        for (int j = me(); j < n; j += nump()) {
            if (y[jy] != 0.0)
            {
                double temp = alpha * y[jy];
                daxpy((i32)m, temp, x, (i32)incx, &a[j * lda], 1);
            }
            jy += incy*nump();
        }
    }
}   //pragma

} 
    } // namespace BLAS
} // namespace CDC8600
