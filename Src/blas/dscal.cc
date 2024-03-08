#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dscal.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dscal(u64 n, f64 a, f64 *x, i64 incx)
        {
            Call(dscal_asm)(n, a, x, incx);
        }

        void dscal_cpp(u64 n, f64 a, f64 *x, i64 incx)
        {
			if(n < 0 || incx < 0) return;

			if(incx == 1)
			{
				u64 M = n % 5;
				if(M != 0)
				{
					for (u64 i = 0; i < M; i++)
					{
						x[i] = a * x[i];
					}
					if(n < 5) return;
				}
				u64 MP1 = M;
				for(u64 i = MP1; i < n; i+= 5)
				{
						x[i] = a * x[i];
						x[i+1] = a * x[i+1];
						x[i+2] = a * x[i+2];
						x[i+3] = a * x[i+3];
						x[i+4] = a * x[i+4];
				}
			}
			else
			{			
				i64 nincx = n*abs(incx);
				i64 ix = 0;				
				while (nincx != 0)			// Any elements left?
				{
					x[ix] = a * x[ix];
					ix += incx;			// Next element of x
					nincx -= incx;				// Decrement element count
				}
			}
        }

	void dscal_asm
	(
	    // u64 n,		[ X0 ]
	    // f64 a,		[ X1 ]
	    // f64 *x,		[ X2 ]
	    // i64 incx,	[ X3 ]	
	)
	{
	    	xkj(4, 0)		// X4 (ix) = 0
			pass()			//																			[2]
	    	jmpn(0, end)	// if X0 (n) < 0 goto end

	    	jmpz(0, end)	// if X0 (n) = 0 goto end
	    	jmpn(3, end)	// if X3 (incx) < 0 goto end

	    	jmpz(3, end)	// if X3 (incx) = 0 goto end			
            rdjk(6, 3)		// X6 (tmp_incx) = X3 (incx)
			idjkj(6, 1)		// X6 (tmp_incx) = X6 (tmp_incx) - 1

			jmpnz(6, loop)	// if X6 (tmp_incx - 1) !=  0 (which means incx != 1)	goto loop
			//INCX = 1 Loop
            rdjk(7, 0)		// X7 (tmp_n) = X0 (n)
	    	xkj(8, 7)		// X8 (seven) = 7 (111)

			lpjkj(7,8)      //  X7 (tmp_n) & X8 (7)  gives the remainder of n%8 (n & 0x07)
			pass()			//																			[2]
			pass()			//																			[2]
			pass()			//																			[2]
LABEL(lt5) 	jmpz(7, DO)	// if X7 (Remainder)  =  0 	goto DO 	(Eight multiplications at a time )
			pass()			//																			[2]
			pass()			//																			[2]

            rdjki(6, 2, 4)	// X6 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(6, 6, 1)		// x[i] = a * X1 (x[i])			
			sdjki(6, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X6 (tmp)
			idjkj(7, 1)		// X7 (Remainder) = X7 (Remainder) - 1

			idjkj(0, 1)		// X0 (n) = X0 (n) - 1
			isjkj(4, 1)		// X4 (ix) = X4 (ix) + 1			
            jmp(lt5)

			//Perform 5 multiplications
LABEL(DO) 	jmpz(0, end)	// if Remainder  =  0 	goto DO
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			isjkj(4, 1)		// X4(ix) = X4 (ix) + 1
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			isjkj(4, 1)		// X4(ix) = X4 (ix) + 1
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			isjkj(4, 1)		// X4(ix) = X4 (ix) + 1
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			isjkj(4, 1)		// X4(ix) = X4 (ix) + 1
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			isjkj(4, 1)		// X4(ix) = X4 (ix) + 1
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			isjkj(4, 1)		// X4(ix) = X4 (ix) + 1
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			isjkj(4, 1)		// X4(ix) = X4 (ix) + 1
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])
			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			isjkj(4, 1)		// X4(ix) = X4 (ix) + 1
			idjkj(0, 8)		// X0 (n) = X0 (n) - 8
			pass()			//																			[2]

            jmp(DO)
			pass()			//																			[2]
			pass()			//																			[2]

LABEL(loop) jmpz(0, end)	// if X0 (n) =  0 	goto end
            rdjki(5, 2, 4)	// X5 (tmp) = MEM[X2 (x) + X4 (ix)]
		    fmul(5, 5, 1)		// x[i] = a * X1 (x[i])

			sdjki(5, 2, 4)	// MEM[X2 (x[i]) + X4 (ix)] = X5 (tmp)
			idjkj(0, 1)		// X0 (n) = X0 (n) - 1
			isjki(4, 4, 3)		// X4 (ix) = X4(ix) + incx
			pass()			//																			[2]

            jmp(loop)
			pass()			//																			[2]
			pass()			//																			[2]
LABEL(end)  jmpk(15, 1)		// return to X15 (calling address) + 1
			pass()			//																			[2]
			pass()			//																			[2]
	}
    } // namespace BLAS
} // namespace CDC8600
