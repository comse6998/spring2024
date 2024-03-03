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
            if (n <= 0 || incx == 0 || lda <= 0 || lda < n)
                return;

            if (incx == 1 ){
                //cout << "here";
                for (int j = lda - 1 ; j >= 1 ; j--){
                    x[j]= x[j] + ddot(j ,a + (j*n), 1, x, 1);
                }
            }
            else if (incx >0 ){
                i64 ix = (n-1)*(incx);
                for (int j = lda - 1 ; j >= 1 ; j--){
                    x[ix]= x[ix] + ddot(j,a + (j* n), 1, x, incx);
                    ix = ix - incx;
                } 
            }
            else{
                i64 ix = 0;
                for (int j = lda - 1 ; j >= 1 ; j--){
                    //cout << "ix => " << ix << endl;
                    x[ix]= x[ix] + ddot(j,a + (j * n), 1, x + ((n-j-lda+1)*(abs(incx))), incx);
                    ix = ix - incx; 
            }}
            

        }
    }
  // namespace BLAS
} // namespace CDC8600
