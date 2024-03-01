#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dgemv_na.hh>
namespace CDC8600
{
extern "C" i32 daxpy_(int32_t *, double *, double *, int32_t *, double *, int32_t *);


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


    for (int i = 0; i<n;i++ ){
    temp1[i] = 0;
    temp2[i] = 0;
    }

    if (m < 0 || n < 0 || (alpha == 0.0 && beta == 0.0) || incx == 0 || incy == 0)
    return;
    //if (incy > 0 && incx>0 ) {
        for (int i = 0; i<m; i++){
            ix = 0;
            if (incx <= 0) ix = (-n+1)*incx;  
            for (int j = 0; j < n; ++j) {
                temp1[i] = temp1[i]+ A[j*m+i]*x[ix];
                ix += incx;
            }
        }
        for (int i = 0; i< m; i++){
            y[iy] = beta*y[iy];
            iy += incy;
        }
        //daxpy_(&M, &beta, y, &INCY, temp2, &one);
        daxpy_(&M, &alpha, temp1, &one, y, &INCY);
       // daxpy_(&M, &fone, temp2, &one, y, &INCY);
    //}

 /*      // Local Variables
     double temp;
     int i, info, ix, j, jy, kx;
     const double zero = 0.0;

     // Test the input parameters
     info = 0;
     if (m < 0)
         info = 1;
     else if (n < 0)
         info = 2;
     else if (incx == 0)
         info = 5;
     else if (incy == 0)
         info = 7;
     else if (lda < std::max(1, int(m)))
         info = 9;

     if (info != 0) {
         char srname[] = "dger  ";
         xerbla_(srname, &info);
         return;
     }

     // Quick return if possible
     if (m == 0 || n == 0 || alpha == zero)
         return;

     // Start the operations
     if (incy > 0)
         jy = 1;
     else
         jy = 1 - (n - 1) * incy;

     if (incx == 1) {
         for (j = 0; j < n; ++j) {
             if (y[jy - 1] != zero) {
                 temp = alpha * y[jy - 1];
                 for (i = 0; i < m; ++i)
                     a[i + j * lda] += x[i] * temp;
             }
             jy += incy;
         }
     } else {
         if (incx > 0)
             kx = 1;
         else
             kx = 1 - (m - 1) * incx;

             //kx = (-m + 1) * incx;


         for (j = 0; j < n; ++j) {
             if (y[jy - 1] != zero) {
                 temp = alpha * y[jy - 1];
                 ix = kx;
                 for (i = 0; i < m; ++i) {
                     a[i + j * lda] += x[ix - 1] * temp;
                     ix += incx;
                 }
             }
             jy += incy;
         }
     } */ 
 }
     } // namespace BLAS
 } // namespace CDC8600