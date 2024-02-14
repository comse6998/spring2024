#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/dasum.hh>

namespace CDC8600
{
    namespace BLAS
    {
        f64 dasum(u64 n, f64 *dx, u64 incx)
        {
            return Func(dasum_cpp)(n, dx, incx);
        }

        f64 dasum_cpp(u64 n, f64 *dx, u64 incx)
        {
            assert(n > 0);
            assert(incx > 0);

            f64 dasum=0;
            f64 dtemp=0;
            u64 i, m;

            if (incx == 1)
            {
                m = n % 6;
                if (m != 0)
                {
                    for (i = 0; i < m; i++)
                        dtemp += std::abs(dx[i]);
                }
                if (n < 6)
                {
                    dasum = dtemp;
                    return dasum;
                }
                for (i = m; i < n; i += 6)
                {
                    dtemp += std::abs(dx[i]) + std::abs(dx[i+1]) + std::abs(dx[i+2]) +
                    std::abs(dx[i+3]) + std::abs(dx[i+4])+ std::abs(dx[i+5]);
                }
            }
            else
            {
                for (i = 0; i < n; i++)
                    dtemp += std::abs(dx[i*incx]);
            }
            dasum = dtemp;
            return dasum;
        }
    }
}