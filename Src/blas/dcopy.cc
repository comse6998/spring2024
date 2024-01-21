#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dcopy.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dcopy(u64 n, f64 *x, i64 incx, f64 *y, i64 incy)
        {
            Call(dcopy_cpp)(n, x, incx, y, incy);
        }

        void dcopy_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy)
        {
	    for (u64 i=0; i<n; i++) y[i * incy] = x[i * incx];
        }

	void dcopy_asm()
	{
	}
    } // namespace BLAS
} // namespace CDC8600
