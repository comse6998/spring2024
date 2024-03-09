#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dgemv_nd.hh>
#include <blas/ddot.hh>
#include <blas/dscal.hh>
#include <cmath>

namespace CDC8600
{
    namespace BLAS
    {
        void dgemv_nd(u64 m, u64 n, f64 alpha, f64* a, u64 lda, f64* x, i64 incx, f64 beta, f64 *y, i64 incy)
        {
            Call(dgemv_nd_cpp)(m, n, alpha, a, lda, x, incx, beta, y, incy);
        }

        void dgemv_nd_cpp(u64 m, u64 n, f64 alpha, f64* a, u64 lda, f64* x, i64 incx, f64 beta, f64 *y, i64 incy)
        {
            if (m <= 0 || n <= 0 || lda < m || incx == 0 || incy == 0){
                return;
            }

            dscal(m, beta, y, abs(incy)); 
            // NOTE, due to the positive only nature of dscal, above only works with abs(incy), 
        
            u64 yIndex;
            if (incy > 0) {
                yIndex = 0; // For positive incy, start at the beginning
            } else {
                yIndex = (m - 1) * abs(incy); // For negative incy, start at the end
            }
            if(incy > 0){
                #pragma omp parallel for 
                for (yIndex = 0; yIndex < m; ++yIndex) {
                    // y index goes through the rows, and saves at specific locations
                    y[yIndex * incy] += alpha * ddot(n, a + yIndex, lda, x, incx);
                }
            } else {
                u64 yStart = (m - 1) * abs(incy);
                #pragma omp parallel for
                for (u64 i = 0; i < m; ++i) {
                    u64 yIndex = yStart - i * abs(incy);   
                    y[yIndex] += alpha * ddot(n, a + i, lda, x, incx);
                }
            }
        }
        void dgemv_nd_asm();
    } // namespace BLAS
} // namespace CDC8600
