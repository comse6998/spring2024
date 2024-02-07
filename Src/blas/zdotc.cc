#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zdotc.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void zdotc(u64 n, c128 *x, i64 incx, c128 *y, i64 incy, c128 *z)
        {
            // Call(zdotc_asm)(n, x, incx, y, incy, z);
        }

        void zdotc_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy, c128 *z)
        {
        c128 ztemp = 0.0;       // Initializing to 0
	    i64 ix = 0;				// First element of x
	    i64 iy = 0;				// First element of y
	    if (incx <= 0) ix = (-n+1)*incx;	// If incx <= 0, start with last element of x
	    if (incy <= 0) iy = (-n+1)*incy;	// If incy <= 0, start with last element of y
	    while (n != 0)			// Any elements left?
	    {
		ztemp = ztemp + std::conj(x[ix]) * y[iy];     //Updating product of the conjugate
		ix += incx;			// Next element of x
		iy += incy;			// Next element of y
		n--;				// Decrement element count
	    }
		*z = ztemp;			// copying dot product to the result
        }

	void zdotc_asm
	(
	    // u64 n,		[ X0 ]
	    // c128 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	    // c128 *y,		[ X3 ]
	    // i64 incy		[ X4 ]
        // c128 *z,     [ X8 ]
	)
	{
	    isjki(2, 2, 2)	// X2 (incx) = 2*X2 (incx)
        isjki(4, 4, 4)	// X4 (incy) = 2*X4 (incy)
	    xkj(5, 0)		// X5 (ix) = 0
	    xkj(6, 0)		// X6 (iy) = 0
        xkj(9,0)        // X9 (ztmp) = 0 (real)
		xkj(10,0)		// X10 (ztmp) = 0 (imaginary)
		xkj(12,0)		// X12 
	    jmpp(2, L1)		// if X2 (incx) > 0 goto L1
	    idzkj(5, 0)		// X5 (ix) = -X0 (n)
	    isjkj(5, 1)		// X5 (ix) = X5(-n) + 1
        ipjkj(5, 2)		// X5 (ix) = X5 (-n+1) * X2 (incx)
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop
	    idzkj(6, 0)		// X6 (iy) = -X0 (n)
	    isjkj(6, 1)		// X6 (iy) = X6(-n) + 1
            ipjkj(6, 4)		// X6 (iy) = X6 (-n+1) * X4 (incy)
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end
        rdjki(7, 1, 5)		// X7 (tmp1) = MEM[X1 (x) + X5 (ix)] (real)
		rdjki(11,3,6)		// X11 (tmp2) = MEM[X3(y) + X6 (iy)] (real) 
		fsubjki(7,12,7)		// X7 (tmp1/x real) = X12 (0) - X7 (tmp1/ x real)
		fmuljki(7,7,11)		// X7 (tmp1) = X7 (tmp1/ x real) * X11 (tmp2/ y real)
		faddjki(9,9,7)		// X9 (ztmp real) = X9 (ztmp real) + X7
	    isjkj(1, 1)		// X1 (x) = X1 (x) + 1
	    isjkj(3, 1)		// X3 (y) = X3 (y) + 1
        rdjki(7, 1, 5)	// X7 (tmp1) = MEM[X1 (x) + X5 (ix)] (imag)
	    rdjki(11,3,6)	// X11 (tmp2) = MEM[X3(y) + X6 (iy)] (imag)
		fmuljki(7,7,11)		// X7 (tmp1) = X7 (tmp1/ x imag) * X11 (tmp2/ y imag)
		faddjki(10,10,7)	// X10 (ztmp imag) = X10 (ztmp imag) + X7
	    idjkj(1, 1)		// X1 (x) = X1 (x) - 1
	    idjkj(3, 1)		// X3 (y) = X3 (y) - 1
        isjki(5, 5, 2)	// X5 (ix) = X5 (ix) + X2 (incx)
	    isjki(6, 6, 4)	// X6 (iy) = X6 (iy) + X4 (incy)
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1
        jmp(loop)
LABEL(end)	sdjki(8, 12, 9)	// MEM[X8 (z) + X12 (0)] = X9 (ztmp real)
		isjkj(12,1)		//X12 (1) = Z12 (0) + 1
		sdjki(8,12,10)	// MEM[X8 (z) + X12 (1)] = X10 (ztmp imag)
		jmpk(15, 1)		// return to X15 (calling address) + 1
	}
    } // namespace BLAS
} // namespace CDC8600
