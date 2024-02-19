#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zdotc.hh>

namespace CDC8600
{
    namespace BLAS
    {
        c128 zdotc(u64 n, c128 *x, i64 incx, c128 *y, i64 incy)
        {
            return Func(zdotc_asm)(n, x, incx, y, incy);
        }

        c128 zdotc_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy)
        {
        c128 z = 0.0;       // Initializing to 0
	    i64 ix = 0;				// First element of x
	    i64 iy = 0;				// First element of y
	    if (incx <= 0) ix = (-n+1)*incx;	// If incx <= 0, start with last element of x
	    if (incy <= 0) iy = (-n+1)*incy;	// If incy <= 0, start with last element of y
	    while (n != 0)			// Any elements left?
	    {
		z = z + std::conj(x[ix]) * y[iy];     //Updating product of the conjugate
		ix += incx;			// Next element of x
		iy += incy;			// Next element of y
		n--;				// Decrement element count
	    }
		// cout<<"cpp z : "<<z<<"\n";
		return z; 
        }

	c128 zdotc_asm
	(
	    // u64 n,		[ X0 ]
	    // c128 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	    // c128 *y,		[ X3 ]
	    // i64 incy		[ X4 ]
	)
	{
	    isjki(2, 2, 2)	// X2 (incx) = 2*X2 (incx)
        isjki(4, 4, 4)	// X4 (incy) = 2*X4 (incy)
	    xkj(5, 0)		// X5 (ix) = 0
	    xkj(6, 0)		// X6 (iy) = 0
        xkj(7,0)        // X7 (ztmp) = 0 (real)
		xkj(8,0)		// X8 (ztmp) = 0 (imag)
	    jmpp(2, L1)		// if X2 (incx) > 0 goto L1
	    idzkj(5, 0)		// X5 (ix) = -X0 (n)
	    isjkj(5, 1)		// X5 (ix) = X5(-n) + 1
        ipjkj(5, 2)		// X5 (ix) = X5 (-n+1) * X2 (incx)
		pass()
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop
	    idzkj(6, 0)		// X6 (iy) = -X0 (n)
	    isjkj(6, 1)		// X6 (iy) = X6(-n) + 1
            ipjkj(6, 4)		// X6 (iy) = X6 (-n+1) * X4 (incy)
		pass()
		pass()
		pass()
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end
        rdjki(9, 1, 5)		// X7 = MEM[X1 (x) + X5 (ix)] (x real)
		rdjki(10,3,6)		// X8 = MEM[X3(y) + X6 (iy)] (y real) 
		// fmuljki(7,7,8)		// X7 (tmp1) = X7 (tmp1/ x real) * X8 (tmp2/ y real)
		// faddjki(9,9,7)		// X9 (ztmp real) = X9 (ztmp real) + X7
		// rdjki(7, 1, 5)		// X7 (tmp1) = MEM[X1 (x) + X5 (ix)] (real)
	    isjkj(1, 1)		// X1 (x) = X1 (x) + 1
	    isjkj(3, 1)		// X3 (y) = X3 (y) + 1
        rdjki(11, 1, 5)	// X11 = MEM[X1 (x) + X5 (ix)] (x imag)
	    rdjki(12,3,6)	// X12 = MEM[X3(y) + X6 (iy)] (y imag)
		fmul(13,9,10) // X13 = X9 * X10
		fadd(7,7,13)     // X7 = X7 + X13
		fmul(13,11,12) // X13 = X11 * X12
		fadd(7,7,13)     // X7 = X7 + X13
		fmul(13,9,12) // x13 = X9*X12
		fadd(8,8,13)     // X8 = X8 + X13
		fmul(13,10,11) // x13 = x10*X11
		fsub(8,8,13)   // X8 = X8 - X13
	    idjkj(1, 1)		// X1 (x) = X1 (x) - 1
	    idjkj(3, 1)		// X3 (y) = X3 (y) - 1
        isjki(5, 5, 2)	// X5 (ix) = X5 (ix) + X2 (incx)
	    isjki(6, 6, 4)	// X6 (iy) = X6 (iy) + X4 (incy)
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1
		pass()
		pass()
		pass()
        jmp(loop)
		pass()
		pass()
LABEL(end)	xkj(1, 0) // X1 = 0
			xkj(0, 0) // X0 = 0
			fadd(0,0,7) // X0 = X0 + X7 (z real)
			fadd(1,1,8) // X1 = X1 + X8 (z imaginary)
		jmpk0(15, 1)		// return to X15 (calling address) + 1
	}
    } // namespace BLAS
} // namespace CDC8600
