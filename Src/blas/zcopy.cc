#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zcopy.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void zcopy(u64 n, c128 *x, i64 incx, c128 *y, i64 incy)
        {
            Call(zcopy_asm)(n, x, incx, y, incy);
        }

        void zcopy_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy)
        {
	    i64 ix = 0;				// First element of x
	    i64 iy = 0;				// First element of y
	    if (incx <= 0) ix = (-n+1)*incx;	// If incx <= 0, start with last element of x
	    if (incy <= 0) iy = (-n+1)*incy;	// If incy <= 0, start with last element of y
	    while (n != 0)			// Any elements left?
	    {
		y[iy] = x[ix];			// Copy element
		ix += incx;			// Next element of x
		iy += incy;			// Next element of y
		n--;				// Decrement element count
	    }
        }

	void zcopy_asm
	(
	    // u64 n,		[ X0 ]
	    // c128 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	    // c128 *y,		[ X3 ]
	    // i64 incy		[ X4 ]
	)
	{
	    // clang-format off
	    isjki(2, 2, 2)	// X2 (incx) = 2*X2 (incx)
            isjki(4, 4, 4)	// X4 (incy) = 2*X4 (incy)
	    xkj(5, 0)		// X5 (ix) = 0
	    xkj(6, 0)		// X6 (iy) = 0
	    jmpp(2, L1)		// if X2 (incx) > 0 goto L1
	    idzkj(5, 0)		// X5 (ix) = -X0 (n)
	    isjkj(5, 1)		// X5 (ix) = X5(-n) + 1
            ipjkj(5, 2)		// X5 (ix) = X5 (-n+1) * X2 (incx)
	    pass()
	    pass()
	    pass()
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop
	    idzkj(6, 0)		// X6 (iy) = -X0 (n)
	    isjkj(6, 1)		// X6 (iy) = X6(-n) + 1
            ipjkj(6, 4)		// X6 (iy) = X6 (-n+1) * X4 (incy)
	    pass()
	    pass()
	    pass()
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end
            rdjki(7, 1, 5)	// X7 (tmp) = MEM[X1 (x) + X5 (ix)] (real)
	    sdjki(7, 3, 6)	// MEM[X3 (y) + X6 (iy)] = X7 (tmp) (real)
	    isjkj(1, 1)		// X1 (x) = X1 (x) + 1
	    isjkj(3, 1)		// X3 (y) = X3 (y) + 1
            rdjki(7, 1, 5)	// X7 (tmp) = MEM[X1 (x) + X5 (ix)] (imag)
	    sdjki(7, 3, 6)	// MEM[X3 (y) + X6 (iy)] = X7 (tmp) (imag)
	    idjkj(1, 1)		// X1 (x) = X1 (x) - 1
	    idjkj(3, 1)		// X3 (y) = X3 (y) - 1
            isjki(5, 5, 2)	// X5 (ix) = X5 (ix) + X2 (incx)
	    isjki(6, 6, 4)	// X6 (iy) = X6 (iy) + X4 (incy)
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1
	    pass()
            jmp(loop)
LABEL(end)  jmpk(15, 1)		// return to X15 (calling address) + 1
	    // clang-format on
	}
    } // namespace BLAS
} // namespace CDC8600
