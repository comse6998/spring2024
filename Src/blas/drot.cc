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
            // 2 Caveat:
            // 1. BR instruction must be the last instruction of the fetch group
            // 2. The BR instruction can only jump to fetch group aligned (8 Byte),
            // which means that it can only jump to the first instruction of a fetch group

            // Optimization 1: If n<=0, return right away.
            // Jmp if zero or negative
            /* instruction                                                 Start/length (in Byte)*/
            pass()                                                      // 0, 2
            pass()                                                      // 2, 2
            jmpz(0, end);                                               // 4, 4
            pass()                                                      // 8, 2
            pass()                                                      // 10, 2
            jmpn(0, end);                                               // 12, 4
            
            // Optimization 2: If incx and incy is both 1, do the bb routine instead of the regular routine.
            xkj(7, 0)		// X7 = 0                                   // 16, 2
            isjki(8, 7, 2);  // Move X2 to X8                           // 18, 2
            idjkj(8, 1);     // X8 = X8 - 1                             // 20, 2
            pass();                                                     // 22, 2
            pass();                                                     // 24, 2
            pass();                                                     // 26, 2
            jmpnz(8, reg);                                              // 28, 4
            isjki(8, 7, 4);    // Move X4 to X8                         // 32, 2 
            idjkj(8, 1);                                                // 34, 2
            jmpnz(8, reg);                                              // 36, 4

            // The optimized routine (incx == incy == 1)
LABEL(opt)  rdjki(9, 1, 7)	// X9 (tmpx) = MEM[X1 (x) + X7 (ix)]        // 40, 2
            rdjki(10, 3, 7)	// X10 (tmpy) = MEM[X3 (y) + X7 (iy)]       // 42, 2
            fmul(11, 5, 9)   	// X11 (tmp1) = X9 (tmpx) * c           // 44, 2
            fmul(12, 6, 10)  	// X12 (tmp2) = X10 (tmpy) * s          // 46, 2
            fadd(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) + X12 (tmp2) // 48, 2
            sdjki(11, 1, 7)	// MEM[X1 (x) + X7 (ix)] = X11 (tmp1)       // 50, 2
            fmul(11, 5, 10)  	// X11 (tmp1) = X10 (tmpy) * c          // 52, 2
            fmul(12, 6, 9)   	// X12 (tmp2) = X9 (tmpx) * s           // 54, 2
            fsub(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) - X12 (tmp2) // 56, 2
            sdjki(11, 3, 7)	// MEM[X3 (y) + X7 (iy)] = X11 (tmp11)      // 58, 2
            isjkj(7, 1)  // X7 = X7 + 1                                 // 60, 2
            bb(7, 0, opt)                                               // 62, 2
            pass()                                                      // 64, 2
            pass()                                                      // 66, 2
            jmp(end)                                                    // 68, 4
            // clang-format off
LABEL(reg)  xkj(7, 0)		// X7 (ix) = 0                              // 72, 2
            xkj(8, 0)		// X8 (iy) = 0                              // 74, 2
            jmpp(2, L1)		// if X2 (incx) > 0 goto L1                 // 76, 4
            idzkj(7, 0)		// X7 (ix) = -X0 (n)                        // 80, 2
            isjkj(7, 1)		// X7 (ix) = X7(-n) + 1                     // 82, 2
            ipjkj(7, 2)		// X7 (ix) = X7 (-n+1) * X2 (incx)          // 84, 2
            pass()                                                      // 86, 2
LABEL(L1)   pass()                                                      // 88, 2
            pass()                                                      // 90, 2
            jmpp(4, loop)	// if X4 (incy) > 0 goto loop               // 92, 4
            idzkj(8, 0)		// X8 (iy) = -X0 (n)                        // 96, 2
            isjkj(8, 1)		// X8 (iy) = X8(-n) + 1                     // 98, 2
            ipjkj(8, 4)		// X8 (iy) = X8 (-n+1) * X4 (incy)          // 100, 2
            pass()                                                      // 102, 2
LABEL(loop) pass()                                                      // 104, 2
            pass()                                                      // 106, 2
            jmpz(0, end)	// if X0 (n) = 0 goto end                   // 108, 4
            rdjki(9, 1, 7)	// X9 (tmpx) = MEM[X1 (x) + X7 (ix)]        // 112, 2
            rdjki(10, 3, 8)	// X10 (tmpy) = MEM[X3 (y) + X8 (iy)]       // 114, 2
            fmul(11, 5, 9)   	// X11 (tmp1) = X9 (tmpx) * c           // 116, 2
            fmul(12, 6, 10)  	// X12 (tmp2) = X10 (tmpy) * s          // 118, 2
            fadd(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) + X12 (tmp2) // 120, 2
            sdjki(11, 1, 7)	// MEM[X1 (x) + X7 (ix)] = X11 (tmp1)       // 122, 2
            fmul(11, 5, 10)  	// X11 (tmp1) = X10 (tmpy) * c          // 124, 2
            fmul(12, 6, 9)   	// X12 (tmp2) = X9 (tmpx) * s           // 126, 2
            fsub(11, 11, 12) 	// X11 (tmp1) = X11 (tmp1) - X12 (tmp2) // 128, 2
            sdjki(11, 3, 8)	// MEM[X3 (y) + X8 (iy)] = X11 (tmp11)      // 130, 2
            isjki(7, 7, 2)	// X7 (ix) = X7 (ix) + X2 (incx)            // 132, 2
            isjki(8, 8, 4)	// X8 (iy) = X8 (iy) + X4 (incy)            // 134, 2
	        idjkj(0, 1)		// X0 (n) = X0 (n) - 1                      // 136, 2
            pass()                                                      // 138, 2
            jmp(loop)                                                   // 140, 4
LABEL(end)  pass()                                                      // 144, 2
            pass()                                                      // 146, 2
            pass()                                                      // 148, 2
            jmpk(15, 1)		// return to X15 (calling address) + 1      // 150, 2

            // clang-format on
	    }
    } // namespace BLAS
} // namespace CDC8600
