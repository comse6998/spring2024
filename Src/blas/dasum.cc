#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dasum.hh>

namespace CDC8600
{
    namespace BLAS
    {
        f64 dasum(u64 n, f64 *x, u64 incx)
        {
            return Func(dasum_asm)(n, x, incx);
        }

        f64 dasum_cpp(u64 n, f64 *x, u64 incx)
        {
            f64 sum=0;
            f64 temp=0;
            u64 i, m;

            if (incx == 1)
            {
                m = n % 6;
                if (m != 0)
                {
                    for (i = 0; i < m; i++)
                        temp += std::abs(x[i]);
                }
                if (n < 6)
                {
                    sum = temp;
                    return sum;
                }
                for (i = m; i < n; i += 6)
                {
                    temp += std::abs(x[i]) + std::abs(x[i+1]) + std::abs(x[i+2]) +
                    std::abs(x[i+3]) + std::abs(x[i+4])+ std::abs(x[i+5]);
                }
            }
            else
            {
                for (i = 0; i < n; i++)
                    temp += std::abs(x[i*incx]);
            }
            sum = temp;
            return sum;
        }

    f64 dasum_asm
    (
        // u64 n,     [ X0 ]
        // f64 *x,    [ X1 ]
        // u64 incx,  [ X2 ]
    )
    {
            // clang-format off
            xkj(4, 0)       // X4 (sum) = 0
            xkj(5, 0)       // X5 (ix) = 0
            pass()          // padding
            pass()
LABEL(loop) jmpz(0, end)    // if X(n) = 0 goto end
            rdjki(6, 1, 5)  // X6 (tmp) = MEM[X1 (x) + X5 (ix)]
            fadd(4, 4, 6)   // X4 (sum) = X4 (sum) + X6 (tmp)
            fadd(5, 5, 2)   // X5 (ix) = X5 (ix) + X2 (incx)
            idjkj(0, 1)     // X0 (n) = X0 (n) - 1
            jmp(loop)
LABEL(end)  xkj(7, 0)
            fadd(0, 4, 7)   // X0 <= X4
            jmpk0(15, 1)    // Return to X15 (calling address) + 1
            // clang-format on
    }
    } // namespace BLAS
} // namespace CDC8600