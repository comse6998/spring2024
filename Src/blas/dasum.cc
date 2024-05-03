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
        // [ X0 ] : n
        // [ X1 ] : x  (array of f64)
        // [ X2 ] : incx
        // [ X4 ] : sum
        // [ X5 ] : ix (index)
        // [ X6 ] : tmp
        // [ X7 ] : tmp
        // [ X8 ] : ret
    )
    {
        // n <= 0, behavior undefined, go to end
            jmpn(0, end)    // if X0 (n) < 0 goto end
            pass()
            pass()
            jmpz(0, end)    // if X0 (n) = 0 goto end
            pass()
            pass()
        
        // if incx = 0, nothing to do, sum = 0, go to end
            jmpz(2, end)    // if X2 (incx) = 0 goto end
            pass()
            pass()

            xkj(4, 0)       // X4 (sum) = 0
            xkj(5, 0)       // X5 (ix) = 0
            pass()          // padding
            pass()

LABEL(loop) jmpn(0, end)
            rdjki(6, 1, 5)  // X6 (tmp) = MEM[X1 (x) + X5 (ix)] Format: [Fijk, 4/4/4/4, length: 4]

            // Take absolute value if necessary, as X(6) could be negative
            xkj(7, 0)       // X7 (tmp) = 0                     Format: [Fjk,  8/4/4, length: 2]
            pass()
            pass()
            jmpn(6, abs)    // if X6 (tmp) < 0 goto abs         Format: [Fjk,  8/4/4, length: 2]

LABEL(back) fadd(4, 4, 6)   // X4 (sum) = X4 (sum) + X6 (tmp)   Format: [Fijk, 4/4/4/4, length: 4]
            fadd(5, 5, 2)   // X5 (ix) = X5 (ix) + X2 (incx)    Format: [Fijk, 4/4/4/4, length: 4]
            idjkj(0, 1)     // X0 (n) = X0 (n) - 1              Format: [Fjk,  8/4/4, length: 2]
            pass()
            pass()
            pass()
            jmp(loop)

LABEL(abs)  fsub(6, 7, 6)   // X6 (tmp) = 0 - X6 (tmp)          Format: [Fijk, 4/4/4/4, length: 4]
            pass()
            jmp(back)       // go back, continue loop           Format: [Fjk,  8/4/4, length: 2]


LABEL(end)  xkj(8, 0)       // X8 (ret) = 0                     Format: [Fijk, 4/4/4/4, length: 4]                               
            fadd(0, 4, 8)   // X0 <= X4                         Format: [Fjk,  8/4/4, length: 2]
            jmpk0(15, 1)    // Return to X15 (calling address) + 1

    }
    } // namespace BLAS
} // namespace CDC8600