#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dcopy.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dcopy(u64 n, f64 *x, i64 incx, f64 *y, i64 incy)
        {
            Call(dcopy_asm)(n, x, incx, y, incy);
        }

        void dcopy_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy)
        {
            i64 ix = 0;                         // First element of x
            i64 iy = 0;                         // First element of y
            if (incx <= 0) ix = (-n+1)*incx;    // If incx <= 0, start with last element of x
            if (incy <= 0) iy = (-n+1)*incy;    // If incy <= 0, start with last element of y
            while (n != 0)                      // Any elements left?
            {
                y[iy] = x[ix];                  // Copy element
                ix += incx;                     // Next element of x
                iy += incy;                     // Next element of y
                n--;                            // Decrement element count
            }
        }

        void dcopy_asm
        (
            // u64 n,           [ X0 ]
            // f64 *x,          [ X1 ]
            // i64 incx,        [ X2 ]
            // f64 *y,          [ X3 ]
            // i64 incy         [ X4 ]
        )
        {
            // clang-format off
            xkj(5, 0)           // X5 (ix) = 0
            xkj(6, 0)           // X6 (iy) = 0
            jmpp(2, L1)         // if X2 (incx) > 0 goto L1
            idzkj(5, 0)         // X5 (ix) = -X0 (n)
            isjkj(5, 1)         // X5 (ix) = X5(-n) + 1
            ipjkj(5, 2)         // X5 (ix) = X5 (-n+1) * X2 (incx)
            pass()              // padding
LABEL(L1)   pass()              // padding
            pass()              // padding
            jmpp(4, loop)       // if X4 (incy) > 0 goto loop
            idzkj(6, 0)         // X6 (iy) = -X0 (n)
            isjkj(6, 1)         // X6 (iy) = X6(-n) + 1
            ipjkj(6, 4)         // X6 (iy) = X6 (-n+1) * X4 (incy)
            pass()              // padding
LABEL(loop) pass()              // padding
            pass()              // padding
            jmpz(0, end)        // if X0 (n) = 0 goto end
            rdjki(7, 1, 5)      // X7 (tmp) = MEM[X1 (x) + X5 (ix)]
            sdjki(7, 3, 6)      // MEM[X3 (y) + X6 (iy)] = X7 (tmp)
            isjki(5, 5, 2)      // X5 (ix) = X5 (ix) + X2 (incx)
            isjki(6, 6, 4)      // X6 (iy) = X6 (iy) + X4 (incy)
            idjkj(0, 1)         // X0 (n) = X0 (n) - 1
            pass()              // padding
            jmp(loop)           // repeat
LABEL(end)  pass()              // padding
            pass()              // padding
            pass()              // padding
            jmpk(15, 1)         // return to X15 (calling address) + 1
            // clang-format on
        }
    } // namespace BLAS
} // namespace CDC8600
