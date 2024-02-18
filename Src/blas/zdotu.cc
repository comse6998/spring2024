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
	    isjki(2, 2, 2)	// X2 (incx) = 2*X2 (incx)                   [2]
	    isjki(4, 4, 4)	// X4 (incy) = 2*X4 (incy)                   [2]
	    xkj(5, 0)		// X5 (ix) = 0                               [2]
	    xkj(6, 0)		// X6 (iy) = 0                               [2]
	    xkj(11,0)           // X11(res-real) = 0                         [2]
	    xkj(12,0)           // X12(res-imag) = 0                         [2]
	    jmpp(2, L1)		// if X2 (incx) > 0 goto L1                  [4]
	    idzkj(5, 0)		// X5 (ix) = -X0 (n)                         [2]
	    isjkj(5, 1)		// X5 (ix) = X5(-n) + 1                      [2]
	    ipjkj(5, 2)		// X5 (ix) = X5 (-n+1) * X2 (incx)           [2]
	    pass()              // padding                                   [2]
LABEL(L1)   jmpp(4, loop)	// if X4 (incy) > 0 goto loop                [4]
	    idzkj(6, 0)		// X6 (iy) = -X0 (n)                         [2]
	    isjkj(6, 1)		// X6 (iy) = X6(-n) + 1                      [2]
	    ipjkj(6, 4)		// X6 (iy) = X6 (-n+1) * X4 (incy)           [2]
	    pass()              // padding                                   [2]
	    pass()              // padding                                   [2]
	    pass()              // padding                                   [2]
LABEL(loop) jmpz(0, end)	// if X0 (n) = 0 goto end                    [4]
	    rdjki(7, 1, 5)	// X7 (tmp) = MEM[X1 (zx) + X5 (ix)] (real)  [2]
	    rdjki(9, 3, 6)	// X9 (tmp) = MEM[X3 (zx) + X6 (iy)] (real)  [2]
	    isjkj(1, 1)		// X1 (zx) = X1 (zx) + 1                     [2]
	    isjkj(3, 1)		// X3 (zy) = X3 (zy) + 1                     [2]
	    rdjki(8, 1, 5)	// X8 (tmp) = MEM[X1 (zx) + X5 (ix)] (imag)  [2]
	    rdjki(10, 3, 6)	// X10(tmp) = MEM[X3 (zx) + X6 (ix)] (imag)  [2]
	    idjkj(1, 1)		// X1 (zx) = X1 (zx) - 1                     [2]
	    idjkj(3, 1)		// X3 (zy) = X3 (zy) - 1                     [2]
	    fmul(13,7,9)        // X13(tmp) = X7 * X9                        [2]
	    fadd(11,11,13)      // X11(res-real) = X11 + X13                 [2]
	    fmul(13,8,10)       // X13(tmp) = X8 * X10                       [2]
	    fsub(11,11,13)      // X11(res-real) = X11 - X13 (because imag)  [2]
	    fmul(13,7,10)       // X13(tmp) = X7 * X10                       [2]
	    fadd(12,12,13)      // X12(res-imag) = X12 + X13                 [2]
	    fmul(13,8,9)        // X13(tmp) = X8 * X9                        [2]
	    fadd(12,12,13)      // X12(res-imag) = X12 + X13                 [2]
	    isjki(5, 5, 2)	// X5 (ix) = X5 (ix) + X2 (incx)             [2]
	    isjki(6, 6, 4)	// X6 (iy) = X6 (iy) + X4 (incy)             [2]
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1                       [2]
	    pass()              // padding                                   [2]
	    jmp(loop)           // loop continue                             [4]
LABEL(end)  isjki(1, 12, 0)     // Optimization, know X0=0, so X1 = X12      [2]
	    isjki(0, 11, 0)     // X0 = X11 + 0                              [2]
	    jmpk(15,1)          // return                                    [4]
	    // clang-format on
        }
    } // namespace BLAS
} // namespace CDC8600
