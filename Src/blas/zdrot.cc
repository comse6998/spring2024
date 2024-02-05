#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zdrot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void zdrot(u64 n, c128 *x, i64 incx, c128 *y, i64 incy, f64 c, f64 s)
        {
			zdrot_cpp(n, x, incx, y, incy, c, s);
            //Call(zdrot_cpp)(n, x, incx, y, incy, c, s);
        }

        void zdrot_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy, f64 c, f64 s)
        {
  // Local Variables
    int i, ix, iy;
    c128 ctemp;

    // Executable Statements
    if (n <= 0)
        return;

    if (incx == 1 && incy == 1) {
        // Code for both increments equal to 1
        for (i = 0; i < n; ++i) {
            ctemp = c * x[i] + s * y[i];
            y[i] = c * y[i] - s * x[i];
            x[i] = ctemp;
        }
    } else {
        // Code for unequal increments or equal increments not equal to 1
        ix = 0;
        iy = 0;
        if (incx < 0)
            ix = (-n + 1) * incx;
        if (incy < 0)
            iy = (-n + 1) * incy;

        for (i = 0; i < n; ++i) {
            ctemp = c * x[ix] + s * y[iy];
            y[iy] = c * y[iy] - s * x[ix];
            x[ix] = ctemp;
            ix += incx;
            iy += incy;
        }
    }
        }

	void zdrot_asm
	(
	    // u64 n,		[ X0 ]
	    // c128 *x,		[ X1 (real), X2(imag) ]
	    // i64 incx,	[ X3 ]
	    // c128 *y,		[ X4(real), X5(imag) ]
	    // i64 incy		[ X6 ]
        // f64 c        [ X7 ]
        // f64 s        [ X8 ]

        // X9 = ctemp.real
        // X10 = ctemp.real
        // X11 = temp
        // X12 = temp
        // X13 = ix / i
        // X14 = iy 
	)
	{

        jmpn(0, end)    // !!! jump if n<0
        jmpz(0, end)
        xkj(13, 0)		// X5 (ix) = 0
        idjki(9, 3, 1)  // !!! X9 = X3 (incx) -1
        idjki(10, 6, 1)  // !!! X19 = X6 (incy) -1
        isjki(9, 9, 10)  //  X7 (incx-1) = X7 (incx-1) + X8(incy-1)
        jmpnz(9, loop2) // !!! jump to loop2 if incx == 0 and incy == 0
LABEL(loop1)  rdjki(1, 1, 5)	// X1 (x.real) = MEM[X1 (x) + X5 (ix)] (real)
        fpjki(9, 1, 7) // !!! X9 (ctemp.real) = X1 (x.real) * X7 (c)
        fpjki(10, 2, 7)     // !!! X10 (ctemp.imag) = X1 (x.imag) * X7 (c)
        fpjki(11, 4, 8)     // !!! X11 =  X4 (y.real) * X8 (s)
        fpjki(12, 5, 8)     // !!! X12 =  X5 (x.imag) * X8 (s)
        fsjki(9, 9, 11)     // !!! X9 (ctemp.real) = X9 (ctemp.real) + X11 (y.real)
        fsjki(10, 10, 11)   // !!! X9 (ctemp.imag) = X9 (ctemp.imag) + X11 (y.imag)
        fpjki(4, 4, 7)      // !!! X4 (y.real) = X4 (y.real) * X7 (c)
        fpjki(5, 5, 7)      // !!! X5 (y.imag) = X5 (y.imag) * X7 (c)
        fpjki(11, 1, 8)     // !!! X11 = X1 (x.real) * X8 (s)
        fpjki(12, 2, 8)     // !!! X12 = X2 (x.imag) * X8 (s)
        fdjki(4, 4, 11)     // !!! X4 (y.real) = X4 (y.real) - X11
        fdjki(5, 5, 12)     // !!! X5 (y.imag) = X5 (y.imag) - X12
        cpjk(1, 9)          // !!! X1 (x.real) = X9 (ctemp.real) 
        cpjk(2, 10)         // !!! X2 (x.imag) = X10 (ctemp.imag)
        idjkj(0, 1)		// X0 (n) = X1 (n) - 1
        jmpz(0, end)       // jump to end if n = 0





	    isjki(2, 2, 2)	// X2 (incx) = 2*X2 (incx)
        isjki(4, 4, 4)	// X4 (incy) = 2*X4 (incy)
	    xkj(5, 0)		// X5 (ix) = 0
	    xkj(6, 0)		// X6 (iy) = 0
	    jmpp(2, L1)		// if X2 (incx) > 0 goto L1
	    idzkj(5, 0)		// X5 (ix) = -X0 (n)
	    isjkj(5, 1)		// X5 (ix) = X5(-n) + 1
        ipjkj(5, 2)		// X5 (ix) = X5 (-n+1) * X2 (incx)
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop
	    idzkj(6, 0)		// X6 (iy) = -X0 (n)
	    isjkj(6, 1)		// X6 (iy) = X6(-n) + 1
            ipjkj(6, 4)		// X6 (iy) = X6 (-n+1) * X4 (incy)
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
            jmp(loop)
LABEL(end)  jmpk(15, 1)		// return to X15 (calling address) + 1
	}
    } // namespace BLAS
} // namespace CDC8600
