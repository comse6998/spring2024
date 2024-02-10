#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/drot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void drot(u64 n, f64 *x, i64 incx, f64 *y, i64 incy, f64 c, f64 s)
        {
            Call(drot_asm)(n, x, incx, y, incy, c, s);
        }

        void drot_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy, f64 c, f64 s)
        {
	    i64 ix = 0;				// First element of x
	    i64 iy = 0;				// First element of y
        f64 temp = 0.0;
	    if (incx <= 0) ix = (-n+1)*incx;	// If incx <= 0, start with last element of x
	    if (incy <= 0) iy = (-n+1)*incy;	// If incy <= 0, start with last element of y
	    while (n != 0)			// Any elements left?
	    {
        temp = c * x[ix] + s * y[iy];
        y[iy] = c * y[iy] - s * x[ix];
		x[ix] = temp;	
		// temp = c * y[iy] - s * x[ix];
        // x[ix] = c * x[ix] + s * y[iy];
		// y[iy] = temp;	
		ix += incx;			// Next element of x
		iy += incy;			// Next element of y
		n--;				// Decrement element count
	    }
        }

	void drot_asm
	(
	    // u64 n,		[ X0 ]
	    // f64 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	    // f64 *y,		[ X3 ]
	    // i64 incy		[ X4 ]
        // f64 c		[ X5 ]
        // f64 s		[ X6 ]
	)
	{
            // clang-format off
            xkj(7, 0)		// X7 (ix) = 0
            xkj(8, 0)		// X8 (iy) = 0
            jmpp(2, L1)		// if X2 (incx) > 0 goto L1
            idzkj(7, 0)		// X7 (ix) = -X0 (n)
            isjkj(7, 1)		// X7 (ix) = X7(-n) + 1
            ipjkj(7, 2)		// X7 (ix) = X7 (-n+1) * X2 (incx)
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop
            idzkj(8, 0)		// X8 (iy) = -X0 (n)
            isjkj(8, 1)		// X8 (iy) = X8(-n) + 1
            ipjkj(8, 4)		// X8 (iy) = X8 (-n+1) * X4 (incy)
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end
            rdjki(9, 1, 7)	// X9 (tmpx) = MEM[X1 (x) + X7 (ix)]
            rdjki(10, 3, 8)	// X10 (tmpy) = MEM[X3 (y) + X8 (iy)]
            fmul(11, 5, 9)   	// X11 (tmp1) = X9 (tmpx) * c
            fmul(12, 6, 10)  	// X12 (tmp2) = X10 (tmpy) * s
            fadd(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) + X12 (tmp2)
            sdjki(11, 1, 7)	// MEM[X1 (x) + X7 (ix)] = X11 (tmp1)
            fmul(11, 5, 10)  	// X11 (tmp1) = X10 (tmpy) * c
            fmul(12, 6, 9)   	// X12 (tmp2) = X9 (tmpx) * s
            fsub(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) - X12 (tmp2)
            sdjki(11, 3, 8)	// MEM[X3 (y) + X8 (iy)] = X11 (tmp11)
            isjki(7, 7, 2)	// X7 (ix) = X7 (ix) + X2 (incx)
            isjki(8, 8, 4)	// X8 (iy) = X8 (iy) + X4 (incy)
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1
            jmp(loop)
LABEL(end)  jmpk(15, 1)		// return to X15 (calling address) + 1
            // clang-format on
	}
    } // namespace BLAS
} // namespace CDC8600
