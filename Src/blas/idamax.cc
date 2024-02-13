#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/idamax.hh>

namespace CDC8600
{
    namespace BLAS
    {
        i32 idamax(u64 n, f64 *x, i64 incx)
        {
            return Call(idamax_cpp)(n, x, incx);
        }

        i32 idamax_cpp(u64 n, f64 *x, i64 incx)
        {
            if (incx <=0 || n < 1){
                return 0;
            }
            i64 ix = 0;	
            double dmax = abs(x[ix]);
            ix += incx;			
            i32 r = 1;
            int i = 2;
            while(i<=n+1){       
                if (abs(x[ix]) >= dmax) {
	                r = i;
                    dmax = x[ix];
                }
                ix += incx;
                i++;
            }
        return r;
        }

	void dcopy_asm
	(
	    // u64 n,		[ X0 ]
	    // f64 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	)
	{
	    xkj(5, 0)		// X5 (ix) = 0
        isjki(5, 5, 2)  // X5 (ix) = X5 (ix) + X2 (incx)
        xkj(6, 2)		// X6 (i) = 2
        idjki(6,6,0)
        idjkj(6, 1)
        xkj(8, 2)		// X7 (r) = 1
        rdjki(7, 1, 5) //X7 (dmax) = MEM[X1 (x) + X5 (ix)]
	    jmpp(2, L1)		// if X2 (incx) > 0 goto L1
        jmp(end)     // if X0 (n) >0 goto end
LABEL(L1)   jmpp(0, loop)     // if X0 (n) >0 goto loop
            jmp(end)  
LABEL(loop) jmpp(6, end)	// if X0 (n) = 0 goto end
            rdjki(8, 1, 5)	// X7 (tmp) = MEM[X1 (x) + X5 (ix)]
            idjki(8,7,8)  // x8 = x7(dmax) - x8 (x[xi])
            jmpp(8, L2)
	        sdjki(7, 3, 6)	// MEM[X3 (y) + X6 (iy)] = X7 (tmp)
            isjki(5, 5, 2)	// X5 (ix) = X5 (ix) + X2 (incx)

LABEL(L2)
	    isjki(6, 6, 4)	// X6 (iy) = X6 (iy) + X4 (incy)
	    idjkj(0, 1)		// X0 (n) = X0 (n) - 1
            jmp(loop)
LABEL(end)  jmpk(15, 1)		// return to X15 (calling address) + 1
	}
    } // namespace BLAS
} // namespace CDC8600
