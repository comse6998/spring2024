#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrmv_utu.hh>
#include <blas/ddot.hh>
#include <blas/dcopy.hh>


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
        
        void dtrmv_utu_cpp(u64 n, f64* a, u64 lda, f64* x, i64 incx){
        
        
        f64 *copyx = (f64*)CDC8600::memalloc(n);
        u64 ix;	
        #pragma omp parallel
        {
            for (u32 i = me(); i < n ; i += nump()){
                dcopy(1, x + ((i)*abs(incx)) , 1, copyx + ((i)*abs(incx)) , 1);
            }
        }
    //    #pragma omp parallel //Blocking
    //     { 
    //         for (u32 i = (n-1) / nump() * me(); i <= ((me() == nump() - 1) ? (n - 1)*abs(incx) : ((n - 1)*abs(incx) / nump() * (me() + 1))); i += 1){
    //             dcopy(1, x + ((i)*abs(incx)) , 1, copyx + ((i)*abs(incx)) , 1);
    //         }
    //     } 
        
        #pragma omp parallel
        {
        for (i32 i = n - me(); i > 0; i = i - nump()){
                if(incx > 0){
                x[(i-1)*incx] = x[(i-1)*incx] + ddot(i-1, copyx, incx, a + ((i-1) * (lda)), 1);
                }
                else{
                x[(-n+i) * (incx)] = x[(-n+i) * (incx)] + ddot(i-1, copyx + ((-n+i-1)*incx), incx, a + ((i-1) * (lda)), 1);
                }
            }
        }
        // #pragma omp parallel //Blocking
        // {
        // for (i32 i = (n - ((n-1) / nump() * me())); i > ((me() == nump() - 1) ? 0 : (n - ((n-1) / nump() * (me()+1)))); i = i - 1){
        //         if(incx > 0){
        //         x[(i-1)*incx] = x[(i-1)*incx] + ddot(i-1, copyx, incx, a + ((i-1) * (lda)), 1);
        //         }
        //         else{
        //         x[(-n+i) * (incx)] = x[(-n+i) * (incx)] + ddot(i-1, copyx + ((-n+i-1)*incx), incx, a + ((i-1) * (lda)), 1);
        //         }
        //     }
        // } 
        
        }
    }
  // namespace BLAS
} // namespace CDC8600
