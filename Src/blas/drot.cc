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
            // Optimization 1: If n<=0, return right away.
            // Jmp if zero or negative
            jmpz(0, end);                                               // 0
            jmpn(0, end);                                               // 4
            
            // Optimization 2: If incx and incy is both 1, do the bb routine instead of the regular routine.
            xkj(7, 0)		// X7 = 0                                   // 8
            isjki(8, 7, 2);  // Move X2 to X8                           // 10
            idjkj(8, 1);     // X8 = X8 - 1                             // 12
            pass()                                                      // 14
            jmpnz(8, reg);                                              // 16
            isjki(8, 7, 4);    // Move X4 to X8                         // 20
            idjkj(8, 1);                                                // 22
            jmpnz(8, reg);                                              // 24
            pass()                                                      // 28
            pass()                                                      // 30

            // The optimized routine (incx == incy == 1)
LABEL(opt)  rdjki(9, 1, 7)	// X9 (tmpx) = MEM[X1 (x) + X7 (ix)]        // 32
            rdjki(10, 3, 7)	// X10 (tmpy) = MEM[X3 (y) + X7 (iy)]       // 34
            fmul(11, 5, 9)   	// X11 (tmp1) = X9 (tmpx) * c           // 36
            fmul(12, 6, 10)  	// X12 (tmp2) = X10 (tmpy) * s          // 38
            fadd(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) + X12 (tmp2) // 40
            sdjki(11, 1, 7)	// MEM[X1 (x) + X7 (ix)] = X11 (tmp1)       // 42
            fmul(11, 5, 10)  	// X11 (tmp1) = X10 (tmpy) * c          // 44
            fmul(12, 6, 9)   	// X12 (tmp2) = X9 (tmpx) * s           // 46
            fsub(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) - X12 (tmp2) // 48
            sdjki(11, 3, 7)	// MEM[X3 (y) + X7 (iy)] = X11 (tmp11)      // 50
            isjkj(7, 1)  // X7 = X7 + 1                                 // 52
            bb(7, 0, opt)                                               // 54
            jmp(end)                                                    // 56
            pass()                                                      // 60
            pass()                                                      // 62
            // clang-format off
LABEL(reg)  xkj(7, 0)		// X7 (ix) = 0                              // 64 (of optimized routine) // 0 (of regular routine)
            xkj(8, 0)		// X8 (iy) = 0                              // 2
            jmpp(2, L1)		// if X2 (incx) > 0 goto L1                 // 4
            idzkj(7, 0)		// X7 (ix) = -X0 (n)                        // 8
            isjkj(7, 1)		// X7 (ix) = X7(-n) + 1                     // 10
            ipjkj(7, 2)		// X7 (ix) = X7 (-n+1) * X2 (incx)          // 12
            pass()                                                      // 14
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop               // 16
            idzkj(8, 0)		// X8 (iy) = -X0 (n)                        // 20
            isjkj(8, 1)		// X8 (iy) = X8(-n) + 1                     // 22
            ipjkj(8, 4)		// X8 (iy) = X8 (-n+1) * X4 (incy)          // 24
            pass()                                                      // 26
            pass()                                                      // 28
            pass()                                                      // 30
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end                   // 32
            rdjki(9, 1, 7)	// X9 (tmpx) = MEM[X1 (x) + X7 (ix)]        // 36
            rdjki(10, 3, 8)	// X10 (tmpy) = MEM[X3 (y) + X8 (iy)]       // 38
            fmul(11, 5, 9)   	// X11 (tmp1) = X9 (tmpx) * c           // 40
            fmul(12, 6, 10)  	// X12 (tmp2) = X10 (tmpy) * s          // 42
            fadd(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) + X12 (tmp2) // 44
            sdjki(11, 1, 7)	// MEM[X1 (x) + X7 (ix)] = X11 (tmp1)       // 46
            fmul(11, 5, 10)  	// X11 (tmp1) = X10 (tmpy) * c          // 48
            fmul(12, 6, 9)   	// X12 (tmp2) = X9 (tmpx) * s           // 50
            fsub(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) - X12 (tmp2) // 52
            sdjki(11, 3, 8)	// MEM[X3 (y) + X8 (iy)] = X11 (tmp11)      // 54
            isjki(7, 7, 2)	// X7 (ix) = X7 (ix) + X2 (incx)            // 56
            isjki(8, 8, 4)	// X8 (iy) = X8 (iy) + X4 (incy)            // 58
	        idjkj(0, 1)		// X0 (n) = X0 (n) - 1                      // 60
            pass()                                                      // 62
            jmp(loop)                                                   // 64
            pass()                                                      // 68
            pass()                                                      // 70
LABEL(end)  jmpk(15, 1)		// return to X15 (calling address) + 1      // 72
            // clang-format on
	    }
    } // namespace BLAS
} // namespace CDC8600
