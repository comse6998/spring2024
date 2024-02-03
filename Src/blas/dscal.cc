#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dscal.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dscal(u64 n, f64 a, f64 *x, i64 incx)
        // void dscal(u64 n, f64 *x, i64 incx, f64 *y, i64 incy)
        {
            Call(dscal_asm)(n, a, x, incx);
        }

        void dscal_cpp(u64 n, f64 a, f64 *x, i64 incx)
        // void dscal_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy)
        {
			i64 nincx = n*abs(incx);
			i64 ix = 0;				
			while (n != 0)			// Any elements left?
			{
				x[ix] = a * x[ix];
				ix += incx;			// Next element of x
				n--;				// Decrement element count
			}
        }

	void dscal_asm
	(
	    // u64 n,		[ X0 ]
	    // f64 a,		[ X1 ]
	    // f64 *x,		[ X2 ]
	    // i64 incx,	[ X3 ]
	)
	{
	    xkj(4, 0)		// X4 (ix) = 0
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    dmul(5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			idjkj(0, 1)		// X0 (n) = X0 (n) - 1
			isjki(4, 4, 3)		// X4 (ix) = X4(ix) + incx
            jmp(loop)
LABEL(end)  jmpk(15, 1)		// return to X15 (calling address) + 1
	}
    } // namespace BLAS
} // namespace CDC8600
