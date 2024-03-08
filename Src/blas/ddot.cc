#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/ddot.hh>
#include <iostream>

namespace CDC8600
{
    namespace BLAS
    {
        f64 ddot(u64 n, f64 *dx, i64 incx, f64 *dy, i64 incy)
        {
            return Func(ddot_asm)(n, dx, incx, dy, incy);
        }

        f64 ddot_cpp(u64 n, f64 *dx, i64 incx, f64 *dy, i64 incy)
        {
            f64 result = 0;

            i64 ix = 0;
            i64 iy = 0;
            if (incx <= 0) ix = (-n + 1) * incx;
            if (incy <= 0) iy = (-n + 1) * incy;

            for (u64 i = 0; i < n; ++i) {
                result += dx[ix] * dy[iy];
                ix += incx;
                iy += incy;
            }

            return result;
        }

    	void ddot_asm
        (
            // u64 n,           [ X0 ]
            // f64 *dx,         [ X1 ]
            // i64 incx,        [ X2 ]
            // f64 *dy,         [ X3 ]
            // i64 incy         [ X4 ]
        )
        {
        // clang-format off
        xkj(7, 0)                                       // X7 (ix) = 0                              //  8/4/4 format
        xkj(8, 0)                                       // X8 (iy) = 0                              //  8/4/4 format
        xkj(11, 0)                                      // X11 (result) = 0.0                       //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        jmpp(2, MATRIX_COMPUTE_START)                   // if X2 (incx) > 0 goto START              //  8/4/4 format
        idzkj(7, 0)                                     // X7 (ix) = -X0 (n)                        //  8/4/4 format
        isjkj(7, 1)                                     // X7 (ix) = X7(-n) + 1                     //  8/4/4 format
        ipjkj(7, 2)                                     // X7 (ix) = X7 (-n+1) * X2 (incx)          //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
LABEL(MATRIX_COMPUTE_START) jmpp(4, MATRIX_COMPUTE_END) // if X4 (incy) > 0 goto MATRIX_COMPUTE_END //  8/4/4 format
        idzkj(8, 0)                                     // X8 (iy) = -X0 (n)                        //  8/4/4 format
        isjkj(8, 1)                                     // X8 (iy) = X8(-n) + 1                     //  8/4/4 format
        ipjkj(8, 4)                                     // X8 (iy) = X8 (-n+1) * X4 (incy)          //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
LABEL(MATRIX_COMPUTE_END) jmpz(0, end)                  // if X0 (n) = 0 goto end                   //  8/4/4 format
        rdjki(9, 1, 7)                                  // X9 (dx value) = MEM[X1 (dx) + X7 (ix)]   //  4/4/4/20 format
        rdjki(10, 3, 8)                                 // X10 (dy value) = MEM[X3 (dy) + X8 (iy)]  //  4/4/4/20 format
        fmul(12, 9, 10)                                 // X12 = X9 * X10                           //  4/4/4/4 format
        fadd(11, 11, 12)                                // X11 (result) += X12                      //  4/4/4/4 format
        isjki(7, 7, 2)                                  // X7 (ix) += X2 (incx)                     //  8/4/4 format
        isjki(8, 8, 4)                                  // X8 (iy) += X4 (incy)                     //  8/4/4 format
        idjkj(0, 1)                                     // X0 (n) -= 1                              //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        jmp(MATRIX_COMPUTE_END)                                                                     //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
        pass()                                                                                      //  8/4/4 format
LABEL(end) isjki(0, 11, 0)     // X0 = X11 + 0                                                      //  8/4/4 format
        jmpk(15, 1)      // return to calling address                                               //  8/4/4 format
        // clang-format on

        }
    } // namespace BLAS
} // namespace CDC8600
