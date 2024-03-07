#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dgemv_na.hh>
#include <blas/daxpy.hh>
namespace CDC8600
{
    namespace BLAS
    {
        void dgemv_na(u64 m, u64 n, f64 alpha, f64* A, u64 lda, f64* x, i64 incx, f64 beta, f64* y, i64 incy)
        {
            //Call(dger_cpp)(m, n, alpha, x, incx, y, incy, a, lda);
            dgemv_na_cpp(m, n, alpha, A, lda, x, incx, beta, y, incy);
        }

void dgemv_na_cpp(u64 m, u64 n, f64 alpha, f64* A, u64 lda, f64* x, i64 incx, f64 beta, f64* y, i64 incy) {

    i32 M = m;
    i32 N = n;
    i32 INCX = incx;
    i32 INCY = incy;
    i32 one = 1;
    f64 fone = 1.0;
    f64* temp1 = (f64*)memalloc(m);
    f64* temp2 = (f64*)memalloc(m);

    i64 ix = 0;                         // First element of x
    i64 iy = 0;                         // First element of y
    if (incx <= 0) ix = (-n+1)*incx;    // If incx <= 0, start with last element of x
    if (incy <= 0) iy = (-m+1)*incy;    // If incy <= 0, start with last element of y


    for (int i = 0; i<m;i++ ){
    temp1[i] = 0;
    temp2[i] = 0;
    }

    if (m < 0 || n < 0 || (alpha == 0.0 && beta == 0.0) || incx == 0 || incy == 0)
    return;
    
    for (int i = 0; i<n; i++){
        daxpy(M, x[ix], &A[i*m], one,  temp2, one);
        ix += incx;
    }
    daxpy(M, alpha, temp2, one,  temp1, one);
    daxpy(M, beta, y, INCY,  temp1, one);
    for (int i = 0; i<m; i++){
        y[iy] = temp1[i];
        iy += incy;
    }
 }
     } // namespace BLAS
 } // namespace CDC8600
