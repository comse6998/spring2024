#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/idamax.hh>

namespace CDC8600
{
    namespace BLAS
    {
        i64 idamax(u64 n, f64 *x, i64 incx)
        {
            return Func(idamax_asm)(n, x, incx);
        }

        i64 idamax_cpp(u64 n, f64 *x, i64 incx)
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

	i64 idamax_asm
	(
	    // u64 n,		[ X0 ]
	    // f64 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	)
	{
            xkj(3, 0)       // X3: zero
            xkj(5, 0)		// X5 (ix) = 0
            rdjki(7, 1, 5) //X7 (dmax) = MEM[X1 (x) + X5 (ix)]
            isjki(5, 5, 2)  // X5 (ix) = X5 (ix) + X2 (incx)
            xkj(6, 2)		// X6 (i) = 2
            xkj(8, 1)		// X8 (r) = 1
            jmpp(7, L0)     // jump to L0 if dmax is positive
            fsub(7, 3, 7)   // else dmax = -dmax (absolute value)
            pass()
            pass()
            pass()
LABEL(L0)	jmpp(2, L1)		// if X2 (incx) > 0 goto L1
            xkj(8, 0)       // else return 0
            pass()
            jmp(end)        //goto end
            pass()
            pass()
LABEL(L1)   jmpp(0, loop)   // if X0 (n) >0 goto loop
            xkj(8, 0)       //else return 0
            pass()
            jmp(end)        // goto end
            pass()
            pass()
LABEL(loop) idjki(9,6,0)    //x9 =  i -n 
            idjkj(9, 1)     // x9 = i - n - 1
            jmpp(9, end)	// if X9 (i-n-1) > 0 goto end
            rdjki(9, 1, 5)	// X9 (tmp) = MEM[X1 (x) + X5 (ix)]
            pass()
            jmpp(9,L2)      //if tmp >0 go to L2
            fsub(9, 3, 9)   //else tmp = -tmp (absolute value)
            pass()
            pass()
            pass()
LABEL(L2)   idjki(9,7,9)    // x9 = x7(dmax) - x9 (x[xi])
            pass()
            jmpp(9, L3)     //if x9 >0 go to L3
            isjki(8, 6, 3)  // else set r (x8) = i (x6)
            rdjki(7, 1, 5)  // set dmax to MEM[X1 (x) + X5 (ix)]
            pass()
            pass()
LABEL(L3)   isjki(5, 5, 2)  // X5 (ix) = X5 (ix) + X2 (incx)
            isjkj(6,1)      // increment i (x6)
            jmp(loop)       // go to start of loop
LABEL(end)  isjki(0, 8, 3)  //set output x0 to r (x8)
            jmpk0(15, 1)	// return to X15 (calling address) + 1
	}
    } // namespace BLAS
} // namespace CDC8600
