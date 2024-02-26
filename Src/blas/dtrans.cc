#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dtrans.hh>
#include <blas/dcopy.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dtrans(i64 m, i64 n, f64 *A, i64 lda, f64 *B, i64 ldb)
        {
            Call(dtrans_cpp)(m, n, A, lda, B, ldb);
        }

        void dtrans_cpp(i64 m, i64 n, f64 *A, i64 lda, f64 *B, i64 ldb)
        {
#pragma omp parallel
	    {
		for (u32 i = me(); i < m; i += nump())
		{
		    dcopy(n, B + i*ldb, 1, A + i, lda);
		}
	    }
        }
    } // namespace BLAS
} // namespace CDC8600
