#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zdotu.hh>

namespace CDC8600
{
    namespace BLAS
    {
        c128 zdotu(u64 n, c128 *zx, i64 incx, c128 *zy, i64 incy)
        {
            return Func(zdotu_asm)(n, zx, incx, zy, incy);
        }

        c128 zdotu_cpp(u64 n, c128 *zx, i64 incx, c128 *zy, i64 incy)
        {
            c128 ztemp = 0;

	    i64 ix = 0;				// First element of zx
	    i64 iy = 0;				// First element of zy
	    if (incx <= 0) ix = (-n+1)*incx;	// If incx <= 0, start with last element of x
	    if (incy <= 0) iy = (-n+1)*incy;	// If incy <= 0, start with last element of y
	    while (n != 0)			// Any elements left?
	    {
                ztemp += zx[ix] * zy[iy];
                ix += incx;
                iy += incy;
		n--;				// Decrement element count
	    }

            return ztemp;           
        }

	void zdotu_asm
        (
            // u64 n,           [ X0 ]
            // c128 *zx,        [ X1 ]
            // i64 incx,        [ X2 ]
            // c128 *zy,        [ X3 ]
            // i64 incy         [ X4 ]
        )
        {
            // clang-format off
	    isjki(2, 2, 2)	// X2 (incx) = 2*X2 (incx)
            isjki(4, 4, 4)	// X4 (incy) = 2*X4 (incy)
	    xkj(5, 0)		// X5 (ix) = 0
	    xkj(6, 0)		// X6 (iy) = 0
            xkj(11,0)           // X11(res-real) = 0
            xkj(12,0)           // X12(res-imag) = 0
	    jmpp(2, L1)		// if X2 (incx) > 0 goto L1
	    idzkj(5, 0)		// X5 (ix) = -X0 (n)
	    isjkj(5, 1)		// X5 (ix) = X5(-n) + 1
            ipjkj(5, 2)		// X5 (ix) = X5 (-n+1) * X2 (incx)
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop
	    idzkj(6, 0)		// X6 (iy) = -X0 (n)
	    isjkj(6, 1)		// X6 (iy) = X6(-n) + 1
            ipjkj(6, 4)		// X6 (iy) = X6 (-n+1) * X4 (incy)
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end
            rdjki(7, 1, 5)	// X7 (tmp) = MEM[X1 (zx) + X5 (ix)] (real)
            rdjki(9, 3, 6)	// X9 (tmp) = MEM[X3 (zx) + X6 (iy)] (real)
	    isjkj(1, 1)		// X1 (zx) = X1 (zx) + 1
	    isjkj(3, 1)		// X3 (zy) = X3 (zy) + 1
            rdjki(8, 1, 5)	// X8 (tmp) = MEM[X1 (zx) + X5 (ix)] (imag)
            rdjki(10, 3, 6)	// X10(tmp) = MEM[X3 (zx) + X6 (ix)] (imag)
	    idjkj(1, 1)		// X1 (zx) = X1 (zx) - 1
	    idjkj(3, 1)		// X3 (zy) = X3 (zy) - 1
            fmul(13,7,9)        // X13(tmp) = X7 * X9
            fadd(11,11,13)      // X11(res-real) = X11 + X13
            fmul(13,8,10)       // X13(tmp) = X8 * X10
            fsub(11,11,13)      // X11(res-real) = X11 - X13 (because imaginary)
            fmul(13,7,10)       // X13(tmp) = X7 * X10
            fadd(12,12,13)      // X12(res-imag) = X12 + X13
            fmul(13,8,9)        // X13(tmp) = X8 * X9
            fadd(12,12,13)      // X12(res-imag) = X12 + X13
            isjki(5, 5, 2)	// X5 (ix) = X5 (ix) + X2 (incx)
	    isjki(6, 6, 4)	// X6 (iy) = X6 (iy) + X4 (incy)
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1
            jmp(loop)
LABEL(end)  isjki(1, 12, 0)     // Optimization, know X0=0, so X1 = X12
            isjki(0, 11, 0)     // X0 = X11 + 0
            jmpk(15,1)
            // clang-format on
        }
    } // namespace BLAS
} // namespace CDC8600
