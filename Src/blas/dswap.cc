#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dswap.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void dswap(u64 n, f64 *x, i64 incx, f64 *y, i64 incy)
        {
            Call(dswap_asm)(n, x, incx, y, incy);
        }

        void dswap_cpp(u64 n, f64 *x, i64 incx, f64 *y, i64 incy)
        {
            i64 ix = 0;                         // First element of x
            i64 iy = 0;
            f64 temp_var = 0;                      // First element of y
            if (incx <= 0) ix = (-n+1)*incx;    // If incx <= 0, start with last element of x
            if (incy <= 0) iy = (-n+1)*incy;    // If incy <= 0, start with last element of y
            while (n != 0)                      // Any elements left?
            {
                temp_var = x[ix] ;
                x[ix] = y[iy];   
                y[iy] = temp_var;

                ix += incx;
                iy += incy;
                n--;
                                        // Decrement element count
            }
        }

        void dswap_asm
        (
            // u64 n,           [ X0 ]
            // f64 *x,          [ X1 ]
            // i64 incx,        [ X2 ]
            // f64 *y,          [ X3 ]
            // i64 incy         [ X4 ]
        )
        {
            xkj(5, 0)           // X5 (ix) = 0
            xkj(6, 0)           // X6 (iy) = 0
            xkj(7, 0)           // X7 (tmp1) = 0
        pass()

            jmpp(2, L1)         // if X2 (incx) > 0 goto L1
            idzkj(5, 0)         // X5 (ix) = -X0 (n)
            isjkj(5, 1)         // X5 (ix) = X5(-n) + 1

            ipjkj(5, 2)         // X5 (ix) = X5 (-n+1) * X2 (incx)
        pass()
        pass()
        pass()

LABEL(L1)   jmpp(4, loop)       // if X4 (incy) > 0 goto loop
            idzkj(6, 0)         // X6 (iy) = -X0 (n)
            isjkj(6, 1)         // X6 (iy) = X6(-n) + 1

            ipjkj(6, 4)         // X6 (iy) = X6 (-n+1) * X4 (incy)
        pass()
        pass()
        pass()

LABEL(loop) jmpz(0, end)        // if X0 (n) = 0 goto end
            rdjki(7, 1, 5)      // X7 (tmp1) = MEM[X1 (x) + X5 (ix)]
            rdjki(8, 3, 6)      // X8 (tmp2) = MEM[X1 (x) + X5 (ix)]

            sdjki(8, 1, 5)      // MEM[X1 (x) + X5 (ix)] = X8 (tmp2)
            sdjki(7, 3, 6)      // MEM[X3 (y) + X6 (iy)] = X7 (tmp1)
            isjki(5, 5, 2)      // X5 (ix) = X5 (ix) + X2 (incx)
            isjki(6, 6, 4)      // X6 (iy) = X6 (iy) + X4 (incy)

            idjkj(0, 1)         // X0 (n) = X0 (n) - 1
        pass()
        pass()
        pass()

            jmp(loop)
        pass()
        pass()

LABEL(end)  jmpk(15, 1)         // return to X15 (calling address) + 1
            // clang-format on
        }
    } // namespace BLAS
} // namespace CDC8600
