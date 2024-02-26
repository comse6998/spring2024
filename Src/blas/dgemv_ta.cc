#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dgemv_ta.hh>
#include <blas/daxpy.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dgemv_ta(u64 m, u64 n, f64 alpha, f64 *A, u64 LDA, f64 *x, i64 incx, f64 beta, f64 *y, i64 incy)
        {
            Call(dgemv_ta_cpp)(m, n, alpha, A, LDA, x, incx, beta, y, incy);
        }

        void dgemv_ta_cpp(u64 m, u64 n, f64 alpha, f64 *A, u64 LDA, f64 *x, i64 incx, f64 beta, f64 *y, i64 incy)
        {
	    if (m <= 0 || n <= 0) return;

            i64 ix = 0;				// First element of x
	    i64 iy = 0;				// First element of y
            
            if (incx <= 0) ix = (-m+1)*incx;	// If incx <= 0, start with last element of x
	    if (incy <= 0) iy = (-n+1)*incy;	// If incy <= 0, start with last element of y
            
            // First scale y by beta
            if (beta != 1.0) 
            {
                for (int i=0;i<n;++i)
                {
                    y[iy] *= beta;
                    iy += incy;
                }
            }
            
            
            for (int i=0;i<m;++i)
            {
                daxpy(n, alpha*x[ix], A+i, LDA, y, incy);
                ix += incx;
            }
            
        }

	void dgemv_ta_asm
	(
	    // u64 n,		[ X0 ]
	    // c128 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	    // c128 *y,		[ X3 ]
	    // i64 incy		[ X4 ]
	)
	{
	    
	}
    } // namespace BLAS
} // namespace CDC8600
