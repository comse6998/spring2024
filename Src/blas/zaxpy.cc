#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zaxpy.hh>

namespace CDC8600
{
	namespace BLAS
	{
		void zaxpy(u64 n, c128 a, c128 *x, i64 incx, c128 *y, i64 incy)
		{
			Call(zaxpy_asm)(n, a, x, incx, y, incy);
		}

		void zaxpy_cpp(u64 n, c128 a, c128 *x, i64 incx, c128 *y, i64 incy)
		{
			if (n <= 0) return;

			if (incx == 1 && incy == 1) {
				for (int i = 0; i < n; i++)
					y[i] += a * x[i];
			} else {
				i64 ix = 0;				// First element of x
				i64 iy = 0;				// First element of y
				if (incx <= 0) ix = (-n+1)*incx;	// If incx <= 0, start with last element of x
				if (incy <= 0) iy = (-n+1)*incy;	// If incy <= 0, start with last element of y
				while (n != 0)			// Any elements left?
				{
					y[iy] += a * x[ix];			// ZY(IY) = ZY(IY) + ZA*ZX(IX)
					ix += incx;			// Next element of x
					iy += incy;			// Next element of y
					n--;				// Decrement element count
				}
			}
		}

		void zaxpy_asm
		(
			// u64 n,           [ X0 ]
			// i64 a.real(),    [ X1 ]
			// i64 a.imag(),    [ X2 ]
			// c128 *x,         [ X3 ]
			// i64 incx,        [ X4 ]
			// c128 *y,         [ X5 ]
			// i64 incy         [ X6 ]
		)
		{
			jmpn(0, end)        // if n < 0 goto end
			jmpz(0, end)        // if n == 0 goto end
			
			xkj(7, 0)           // X7 (tmp) = 0
			idjkj(7, 1)         // X7 (tmp) = -1
			isjki(7, 7, 4)      // X7 (tmp) = X7 (-1) + X4 (incx)
			jmpnz(7, general)   // if X4 (incx) != 1 goto general
			idjkj(7, 1)         // X7 (tmp) = -1
			isjki(7, 7, 6)      // X7 (tmp) = X7 (-1) + X6 (incy)
			jmpnz(7, general)   // if X6 (incy) != 1 goto general

			/* optimization */
			idjkj(0, 1)         // X0 (n) = X0 (n) - 1

LABEL(opt_loop)
			jmpn(0, end)        // if X0 (n) < 0 goto end

			isjki(8, 0, 0)      // X8 (i) = 2 * X0 (n)
			rdjki(9, 5, 8)      // X9 (tmp1) = MEM[X5 (y) + X8 (i)] (y.real)
			rdjki(10, 3, 8)     // X10 (tmp2) = MEM[X3 (x) + X8 (i)] (x.real)
			fmul(10, 10, 1)     // X10 (tmp2) = X10 (x.real) * X1 (a.real)
			fadd(9, 9, 10)      // X9 (tmp1) = X9 (y.real) + X10 (x.real * a.real)
			isjkj(3, 1)         // X3 (x) = X3 (x) + 1
			rdjki(10, 3, 8)     // X10 (tmp2) = MEM(X3 (x) + X8 (i)) (x.imag)
			fmul(10, 10, 2)     // X10 (tmp2) = X(10) (x.imag) * X2 (a.imag)
			fsub(9, 9, 10)      // X9 (tmp1) = X9 (y.real + x.real * a.real) - X10 (x.imag * a.imag)
			sdjki(9, 5, 8)      // MEM[X5 (y) + X8 (i)] (y.real) = X9 (tmp1)

			isjkj(5, 1)         // X5 (y) = X5 (y) + 1
			rdjki(9, 5, 8)      // X9 (tmp1) = MEM(X5 (y) + X8 (i)) (y.imag)
			rdjki(10, 3, 8)     // X10 (tmp2) = MEM(X3 (x) + X8 (i)) (x.imag)
			fmul(10, 10, 1)     // X10 (tmp2) = X10 (x.imag) * X1 (a.real)
			fadd(9, 9, 10)      // X9 (tmp1) = X9 (y.imag) + X10 (x.imag * a.real)
			idjkj(3, 1)         // X3 (x) = X3 (x) - 1
			rdjki(10, 3, 8)     // X10 (tmp2) = MEM[X3 (x) + X8 (i)] (x.real)
			fmul(10, 10, 2)     // X10 (tmp2) = X10 (x.real) * X2 (a.imag)
			fadd(9, 9, 10)      // X9 (tmp1) = X9 (y.imag + x.imag * a.real) + X10 (x.imag * a.real)
			sdjki(9, 5, 8)      // MEM[X5 (y) + X8 (i)] (y.imag) = X9 (tmp1)
			idjkj(5, 1)         // X5 (y) = X5 (y) - 1

			idjkj(0, 1)         // X0 (n) = X0 (n) - 1
			jmp(opt_loop)

			/* general cases */ 
LABEL(general)
			isjki(4, 4, 4)      // X4 (incx) = 2 * X4 (incx)
			isjki(6, 6, 6)      // X6 (incy) = 2 * X6 (incy)
			xkj(7, 0)           // X7 (ix) = 0
			xkj(8, 0)           // X8 (iy) = 0
			jmpp(4, L1)         // if X4 (incx) > 0 goto L1
			idzkj(7, 0)         // X7 (ix) = -X0 (n)
			isjkj(7, 1)         // X7 (ix) = X7(-n) + 1
			ipjkj(7, 4)         // X7 (ix) = X7 (-n+1) * X4 (incx)
LABEL(L1)	
			jmpp(6, loop)       // if X6 (incy) > 0 goto loop
			idzkj(8, 0)         // X8 (iy) = -X0 (n)
			isjkj(8, 1)         // X8 (iy) = X8(-n) + 1
			ipjkj(8, 6)         // X8 (iy) = X8 (-n+1) * X6 (incy)
LABEL(loop)	
			jmpz(0, end)        // if X0 (n) = 0 goto end
			rdjki(9, 5, 8)      // X9 (tmp1) = MEM[X5 (y) + X8 (iy)] (y.real)
			rdjki(10, 3, 7)     // X10 (tmp2) = MEM[X3 (x) + X7 (ix)] (x.real)
			fmul(10, 10, 1)     // X10 (tmp2) = X10 (x.real) * X1 (a.real)
			fadd(9, 9, 10)      // X9 (tmp1) = X9 (y.real) + X10 (x.real * a.real)
			isjkj(3, 1)         // X3 (x) = X3 (x) + 1
			rdjki(10, 3, 7)     // X10 (tmp2) = MEM[X3 (x) + X7 (ix)] (x.imag)
			fmul(10, 10, 2)     // X10 (tmp2) = X10 (x.imag) * X2 (a.imag)
			fsub(9, 9, 10)      // X9 (tmp1) = X9 (y.real + x.real * a.real) - X10 (x.imag * a.imag)
			sdjki(9, 5, 8)      // MEM[X5 (y) + X8 (iy)] (y.real) = X9 (tmp1)

			isjkj(5, 1)         // X5 (y) = X5 (y) + 1
			rdjki(9, 5, 8)      // X9 (tmp1) = MEM(X5 (y) + X8 (iy)) (y.imag)
			rdjki(10, 3, 7)     // X10 (tmp2) = MEM(X3 (x) + X7 (ix)) (x.imag)
			fmul(10, 10, 1)     // X10 (tmp2) = X10 (x.imag) * X1 (a.real)
			fadd(9, 9, 10)      // X9 (tmp1) = X9 (y.imag) + X10 (x.imag * a.real)
			idjkj(3, 1)         // X3 (x) = X3 (x) - 1
			rdjki(10, 3, 7)     // X10 (tmp2) = MEM[X3 (x) + X7 (ix)] (x.real)
			fmul(10, 10, 2)     // X10 (tmp2) = X10 (x.real) * X2 (a.imag)
			fadd(9, 9, 10)      // X9 (tmp1) = X9 (y.imag + x.imag * a.real) + X10 (x.imag * a.real)
			sdjki(9, 5, 8)      // MEM[X5 (y) + X8 (iy)] (y.imag) = X9 (tmp1)
			idjkj(5, 1)         // X5 (y) = X5 (y) - 1

			isjki(7, 7, 4)      // X7 (ix) = X7 (ix) + X4 (incx)
			isjki(8, 8, 6)      // X8 (iy) = X8 (iy) + X6 (incy)
			idjkj(0, 1)         // X0 (n) = X0 (n) - 1
			jmp(loop)

LABEL(end)	
			jmpk(15, 1)         // return to X15 (calling address) + 1
		}
	} // namespace BLAS
} // namespace CDC8600
