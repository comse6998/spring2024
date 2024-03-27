#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zswap.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void zswap(u64 n, c128 *x, i64 incx, c128 *y, i64 incy)
        {
            Call(zswap_asm)(n, x, incx, y, incy);
        }

        void zswap_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy)
        {
	    i64 ix = 0;				// First element of x
	    i64 iy = 0;				// First element of y
	    c128 temp = 0;
            if (incx == 1 && incy ==1)
            {
                for (long unsigned i=0; i<n; i++)
                {
                    temp = x[i];
                    x[i] = y[i];
                    y[i] = temp;
                }
            }
            else
            {
                if (incx <= 0) ix = (-n+1)*incx;	// If incx <= 0, start with last element of x
	        if (incy <= 0) iy = (-n+1)*incy;	// If incy <= 0, start with last element of y
	        while (n != 0)			// Any elements left?
	        {
	            temp = x[ix];
                    x[ix] = y[iy];			// Swap element
	            y[iy] = temp;
                    ix += incx;			// Next element of x
	            iy += incy;			// Next element of y
	            n--;				// Decrement element count
	        }
            }
        }

	void zswap_asm
	(
	    // u64 n,		[ X0 ]
	    // c128 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	    // c128 *y,		[ X3 ]
	    // i64 incy		[ X4 ]
	)
	{
	    idjkj(2, 1)         // X2 (incx) = X2 (incx) - 1
	    pass()		// padding
            jmpnz(2, INC1)      // if X6 (tmp) != 0 goto gen
	    idjkj(4, 1)         // X4 (incy) = X4 (incy) - 1
	    pass()		// padding
            jmpnz(4, INC2)      // if X6 (tmp) != 0 goto gen
LABEL(uinc) jmpz(0, end)	// w if X0 (n) = 0 goto end; X6 (i) = 0
            rdjki(7, 1, 6)	// X7 (temp) = MEM[X1 (x) + X6 (i)] (real)
            rdjki(8, 3, 6)	// X8 (tmp) = MEM[X3 (y) + X6 (i)] (real)
	    sdjki(8, 1, 6)	// w MEM[X1 (x) + X6 (i)] = X8 (tmp) (real)
	    sdjki(7, 3, 6)	// MEM[X3 (y) + X6 (i)] = X7 (temp) (real)
            isjkj(1, 1)		// X1 (x) = X1 (x) + 1
	    isjkj(3, 1)		// X3 (y) = X3 (y) + 1
            rdjki(7, 1, 6)	// w X7 (temp) = MEM[X1 (x) + X6 (i)] (imag)
            rdjki(8, 3, 6)	// X8 (tmp) = MEM[X3 (y) + X6 (i)] (imag)
	    sdjki(8, 1, 6)	// MEM[X1 (x) + X6 (i)] = X8 (tmp) (imag)
	    sdjki(7, 3, 6)	// MEM[X3 (y) + X6 (i)] = X7 (temp) (imag)
            idjkj(1, 1)		// w X1 (x) = X1 (x) - 1
	    idjkj(3, 1)		// X3 (y) = X3 (y) - 1
            isjkj(6, 2)	        // X6 (i) = X6 (i) + 2
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1
            jmp(uinc)           // w 
	    pass()		// padding
	    pass()		// padding
LABEL(INC2) isjkj(4,1)          // X4(incy) = X4(incy) + 1
	    pass()		// padding
	    pass()		// padding
	    pass()		// padding
LABEL(INC1) isjkj(2,1)          // X2(incx) = X2(incx) + 1
            isjki(2, 2, 2)	// X2 (incx) = 2*X2 (incx)
            isjki(4, 4, 4)	// X4 (incy) = 2*X4 (incy)
	    xkj(5, 0)		// X5 (ix) = 0
	    xkj(6, 0)		// X6 (iy) = 0
	    pass()		// padding
	    jmpp(2, L1)		// if X2 (incx) > 0 goto L1
	    idzkj(5, 0)		// X5 (ix) = -X0 (n)
	    isjkj(5, 1)		// X5 (ix) = X5(-n) + 1
            ipjkj(5, 2)		// X5 (ix) = X5 (-n+1) * X2 (incx)
	    pass()		// padding
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop
	    idzkj(6, 0)		// X6 (iy) = -X0 (n)
	    isjkj(6, 1)		// X6 (iy) = X6(-n) + 1
            ipjkj(6, 4)		// X6 (iy) = X6 (-n+1) * X4 (incy)
	    pass()		// padding
	    pass()		// padding
	    pass()		// padding
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end
            rdjki(7, 1, 5)	// X7 (temp) = MEM[X1 (x) + X5 (ix)] (real)
            rdjki(8, 3, 6)	// X8 (tmp) = MEM[X3 (y) + X6 (iy)] (real)
	    sdjki(8, 1, 5)	// MEM[X1 (x) + X5 (ix)] = X8 (tmp) (real)
	    sdjki(7, 3, 6)	// MEM[X3 (y) + X6 (iy)] = X7 (temp) (real)
            isjkj(1, 1)		// X1 (x) = X1 (x) + 1
	    isjkj(3, 1)		// X3 (y) = X3 (y) + 1
            rdjki(7, 1, 5)	// X7 (temp) = MEM[X1 (x) + X5 (ix)] (imag)
            rdjki(8, 3, 6)	// X8 (tmp) = MEM[X3 (y) + X6 (iy)] (imag)
	    sdjki(8, 1, 5)	// MEM[X1 (x) + X5 (ix)] = X8 (tmp) (imag)
	    sdjki(7, 3, 6)	// MEM[X3 (y) + X6 (iy)] = X7 (temp) (imag)
            idjkj(1, 1)		// X1 (x) = X1 (x) - 1
	    idjkj(3, 1)		// X3 (y) = X3 (y) - 1
            isjki(5, 5, 2)	// X5 (ix) = X5 (ix) + X2 (incx)
	    isjki(6, 6, 4)	// X6 (iy) = X6 (iy) + X4 (incy)
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1
	    pass()		// padding
            jmp(loop)            
LABEL(end)  jmpk(15, 1)		// return to X15 (calling address) + 1
	}
    } // namespace BLAS
} // namespace CDC8600
