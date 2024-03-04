#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dgemv_ta.hh>
#include <blas/daxpy.hh>
#include <blas/dscal.hh>

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
	    
#pragma omp parallel
            {
	        i64 incy_abs = abs(incy);               // Absolute value of incy
                
                u32 mod = n % nump();
                u32 k = 0;
                if (me() < mod) k = 1;
                
                // First scale y by beta
                if (beta != 1.0) 
                {
                    dscal(u64(n/nump()) + k, beta, y + me()*incy_abs, nump()*incy_abs);
                }
            }

// separate dscal and daxpy sections to synchronize threads
#pragma omp parallel
            {
                
                i64 ix = 0;				// First element of x
                if (incx <= 0) ix = (-m+1)*incx;        // If incx <= 0, start with last element of x
                u32 mod = n % nump();
                u32 k = 0;
                if (me() < mod) k = 1;
                
                f64 *y_temp;
                if (incy <= 0) y_temp = y - (mod - 1 + nump()*(1 - k))*incy; else y_temp = y;

                for (u64 i = 0; i < m; ++i)
                {
                    daxpy
                    (
                    u64(n/nump()) + k,    alpha*x[ix], 
                    A + me()*LDA + i,   nump()*LDA, 
                    //y + me()*incy,      nump()*incy
                    y_temp + me()*incy,      nump()*incy
                    );
                    
                    ix += incx;
                }
            
            } 
            
            // =========== single-thread
            
            //if (m <= 0 || n <= 0) return;

            //i64 ix = 0;				// First element of x
	    //i64 incy_abs = abs(incy);		// Absolute value of incy
            //
            //if (incx <= 0) ix = (-m+1)*incx;	// If incx <= 0, start with last element of x
            //
            //// First scale y by beta
            //if (beta != 1.0) 
            //{
            //    dscal(n, beta, y, incy_abs);
            //}
            //
            //for (u64 i=0;i<m;++i)
            //{
            //    daxpy(n, alpha*x[ix], A+i, LDA, y, incy);
            //    ix += incx;
            //}
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
