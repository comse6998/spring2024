#ifndef _dtrans_HH_
#define _dtrans_HH_

#include <CDC8600.hh>

namespace CDC8600
{
    namespace BLAS
    {
        // y = x
        void dtrans    (i64 m, i64 n, f64 *A, i64 lda, f64 *B, i64 ldb);
        void dtrans_cpp(i64 m, i64 n, f64 *A, i64 lda, f64 *B, i64 ldb);
	void dtrans_asm();
    } // namespace BLAS
}; // namespace CDC8600

#endif
