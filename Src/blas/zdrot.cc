#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zdrot.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void zdrot(u64 n, c128 *x, i64 incx, c128 *y, i64 incy, f64 c, f64 s)
        {
			
            Call(zdrot_asm)(n, x, incx, y, incy, c, s);
        }

        void zdrot_cpp(u64 n, c128 *x, i64 incx, c128 *y, i64 incy, f64 c, f64 s)
        {
  // Local Variables
    i64 i, ix, iy;
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

        while ( n != 0) 
        {
            ctemp = c * x[ix] + s * y[iy];
            y[iy] = c * y[iy] - s * x[ix];
            x[ix] = ctemp;
            ix += incx;
            iy += incy;
            n--;
        }
    }
        }

	void zdrot_asm
	(
	    // u64 n,		[ X0 ]
	    // c128 *x,		[ X1 ]
	    // i64 incx,	[ X2 ]
	    // c128 *y,		[ X3 ]
	    // i64 incy		[ X4 ]
        // f64 c        [ X5 ]
        // f64 s        [ X6 ]
        // i / ix       [ X7 ]
        // iy           [ X8 ]
        // ctemp        [ X9 ]
        // ctemp        [ X10]
        // temp1        [ X11]
        // temp2        [ X12]
        // temp3        [ X13] 
	)
	{ 
        //jmpn(0, end)          // !!! jump if n<0
       jmpz(0, end)         
        xkj(7, 0)               // X7 (ix) == 0
 
        xkj(12, 0)              // X12 = 0
        isjki(9, 2, 12)          // X9 = incx (X2) + 0 (X12) 
        isjki(10, 4, 12)         // X10 = incy (X4) + 0 (X12) 
        idjkj(9, 1)
        pass()		        // X9 (incx) = X9 (incx) - 2
        jmpnz(9, start_r)
        idjkj(10, 1)		    // X10 (incy) = X10 (incy) - 2
        pass()
        jmpnz(10, start_r)

//Optimized Version using backward branch
        //idjkj(0, 1)	            // X0 (n) = X0 (n) -1
        xkj(13, 2)
        ipjkj(0, 13)		    // X0 (n) = X2 (n) * 2

        //REAL PART
LABEL(start_o)rdjki(11, 1, 7)	        // X11 (x.real, temp1) = MEM[X1 (x) + X7 (i)] (real)
        //REAL PART
        rdjki(11, 1, 7)	        // X11 (x.real, temp1) = MEM[X1 (x) + X7 (i)] (real)
        rdjki(12, 3, 7)	        // X12 (y.real, temp2) = MEM[X3 (y) + X7 (i)] (real)
        
        fmul(9, 11, 5)          // X9 (ctemp.real) =  X11 (x.real, temp1) * X5 (c) 
        fmul(13, 12, 6)         // X13 (temp3) =  X12 (y.real, temp2) * X6 (s) 
        fadd(9, 9, 13)          // X9 (ctemp.real) = X9 (ctemp.real) + X13 (temp3)
        
        fmul(12, 12, 5)         // X12 (y.real) =  X12 (y.real, temp1) * X5 (c) 
        fmul(13, 11, 6)         // X13 (temp3) =  X11 (x.real, temp2) * X6 (s) 
        fsub(12, 12, 13)        // X12 (ctemp.real) = X12 (y.real) - X13 (temp3)

        //cpjk(11, 9)           // X11 (x.real) = X9 (ctemp.real)
        xkj(13, 0)              
        isjki(11, 9, 13)  

        sdjki(11, 1, 7)	        // MEM[X1 (x) + X7 (i)] = X11 (tmp) (real)
        sdjki(12, 3, 7)	        // MEM[X3 (y) + X7 (i)] = X12 (tmp) (real)
//END REAL PART

//IMAGINARY PART
        isjkj(7, 1)		        // X7 (i) = X7 (i) + 1, increment ix to get imaginary part of number

        rdjki(11, 1, 7)	        // X11 (x.imag, temp1) = MEM[X1 (x) + X7 (i)] (imag)
        rdjki(12, 3, 7)	        // X12 (y.imag, temp2) = MEM[X3 (y) + X7 (i)] (imag)
        
        fmul(9, 11, 5)          // X9 (ctemp.imag) =  X11 (x.imag, temp1) * X5 (c) 
        fmul(13, 12, 6)         // X13 (temp3) =  X12 (y.imag, temp2) * X6 (s) 
        fadd(9, 9, 13)          // X9 (ctemp.imag) = X9 (ctemp.imag) + X13 (temp3)
        
        fmul(12, 12, 5)         // X12 (y.imag) =  X12 (y.imag, temp1) * X5 (c) 
        fmul(13, 11, 6)         // X13 (temp3) =  X11 (x.imag, temp2) * X6 (s) 
        fsub(12, 12, 13)        // X12 (ctemp.imag) = X12 (y.imag) - X13 (temp3)

        //cpjk(11, 9)           // X11 (x.imag) = X9 (ctemp.imag)
        xkj(13, 0)              
        isjki(11, 9, 13)           


        sdjki(11, 1, 7)	        // MEM[X1 (x) + X7 (i)] = X11 (tmp) (imag)
        sdjki(12, 3, 7)	        // MEM[X3 (y) + X7 (i)] = X12 (tmp) (imag)
//END IMAGINARY PART

        isjkj(7, 1)	            // X7 (i) = X7 (i) + 1
        //idjkj(0, 1)		    // X0 (n) = X0 (n) - 1
        //jmp(start_o)          // jump to beginning of the loop
        bb(7, 0, start_o)       //jump to start of the loop if X0 (n) - X7 (i) >= 0      
        jmp(end)

//END OPTIMIZATION

//START REGULAR
        pass()
        pass()
LABEL(start_r)  xkj(8, 0)               // X8 (iy) == 0
        xkj(13, 2)
        ipjkj(2, 13)		    // X2 (incx) = X2 (incx) * 2
        ipjkj(4, 13)		    // X4 (incx) = X4 (incx) * 2
        //test for increments < 0
LABEL(start)jmpp(2, check)      //skip this section if incx > 0
        idzkj(7, 0)		        // X7 (ix) = -X0 (n)
	isjkj(7, 1)		        // X7 (ix) = X7(-n) + 1
        ipjkj(7, 2)	
        pass()	        // X7 (ix) = X7 (-n+1) * X2 (incx)
        pass()
        pass()

LABEL(check)jmpp(4, loop1)      //skip this section if incy > 0
        idzkj(8, 0)		        // X8 (iy) = -X0 (n)
        isjkj(8, 1)		        // X8 (iy) = X8(-n) + 1
        ipjkj(8, 4)
        pass()		        // X8 (iy) = X8 (-n+1) * X4 (incy)
        pass()
        pass() 
LABEL(loop1) jmpz(0, end)       // jump to end if X0 (n) == 0   
//REAL PART
        rdjki(11, 1, 7)	        // X11 (x.real, temp1) = MEM[X1 (x) + X7 (ix)] (real)
        rdjki(12, 3, 8)	        // X12 (y.real, temp2) = MEM[X3 (y) + X8 (iy)] (real)
        
        fmul(9, 11, 5)          // X9 (ctemp.real) =  X11 (x.real, temp1) * X5 (c) 
        fmul(13, 12, 6)         // X13 (temp3) =  X12 (y.real, temp2) * X6 (s) 
        fadd(9, 9, 13)          // X9 (ctemp.real) = X9 (ctemp.real) + X13 (temp3)
        
        fmul(12, 12, 5)         // X12 (y.real) =  X12 (y.real, temp1) * X5 (c) 
        fmul(13, 11, 6)         // X13 (temp3) =  X11 (x.real, temp2) * X6 (s) 
        fsub(12, 12, 13)        // X12 (ctemp.real) = X12 (y.real) - X13 (temp3)

        //cpjk(11, 9)           // X11 (x.real) = X9 (ctemp.real)
        xkj(13, 0)              
        isjki(11, 9, 13)  

        sdjki(11, 1, 7)	        // MEM[X1 (x) + X7 (ix)] = X11 (tmp) (real)
        sdjki(12, 3, 8)	        // MEM[X3 (y) + X8 (iy)] = X12 (tmp) (real)
//END REAL PART

//IMAGINARY PART
        isjkj(7, 1)		        // X7 (ix) = X7 (ix) + 1, increment ix to get imaginary part of number
        isjkj(8, 1)		        // X8 (iy) = X8 (iy) + 1, increment iy to get imaginary part of number

        rdjki(11, 1, 7)	        // X11 (x.imag, temp1) = MEM[X1 (x) + X7 (ix)] (imag)
        rdjki(12, 3, 8)	        // X12 (y.imag, temp2) = MEM[X3 (y) + X8 (iy)] (imag)
        
        fmul(9, 11, 5)          // X9 (ctemp.imag) =  X11 (x.imag, temp1) * X5 (c) 
        fmul(13, 12, 6)         // X13 (temp3) =  X12 (y.imag, temp2) * X6 (s) 
        fadd(9, 9, 13)          // X9 (ctemp.imag) = X9 (ctemp.imag) + X13 (temp3)
        
        fmul(12, 12, 5)         // X12 (y.imag) =  X12 (y.imag, temp1) * X5 (c) 
        fmul(13, 11, 6)         // X13 (temp3) =  X11 (x.imag, temp2) * X6 (s) 
        fsub(12, 12, 13)        // X12 (ctemp.imag) = X12 (y.imag) - X13 (temp3)

        //cpjk(11, 9)           // X11 (x.imag) = X9 (ctemp.imag)
        xkj(13, 0)              
        isjki(11, 9, 13)           


        sdjki(11, 1, 7)	        // MEM[X1 (x) + X7 (ix)] = X11 (tmp) (imag)
        sdjki(12, 3, 8)	        // MEM[X3 (y) + X8 (iy)] = X12 (tmp) (imag)

        idjkj(7, 1)		        // X7 (ix) = X7 (ix) - 1, decrement ix for real part
        idjkj(8, 1)		        // X8 (iy) = X8 (iy) - 1, decrement iy for real part
//END IMAGINARY PART

        isjki(7, 7, 2)	        // X7 (ix) = X7 (ix) + X2 (incx)
        isjki(8, 8, 4)	        // X8 (iy) = X8 (iy) + X4 (incy)

        idjkj(0, 1)
        pass()		        // X0 (n) = X0 (n) - 1
        jmp(loop1)              // jump to beginning of the loop


LABEL(end)jmpk(15, 1)           // return to X15 (calling address) + 1


	    }
    } // namespace BLAS
} // namespace CDC8600
