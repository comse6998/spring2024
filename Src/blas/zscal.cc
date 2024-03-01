#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/zscal.hh>

namespace CDC8600
{
    namespace BLAS
    {
        void zscal(i64 n, c128 a, c128 *x, i64 incx)
        {
            Call(zscal_asm)(n, a, x, incx);
        }

        void zscal_cpp(i64 n, c128 a, c128 *x, i64 incx)
        {
            if(n < 0 || incx < 0) return;
            if(incx==1){
                for(int i=0; i<n; ++i) x[i] = a*x[i];
            }
            else{
                for(int i=0; i<(n*incx); i += incx) x[i] = a*x[i];
            }
            return;
        }

        void zscal_asm
        (
            // i64 n,           [ X0 ]
            // f64 a.real(),    [ X1 ]
            // f64 a.imag(),    [ X2 ]
            // c128 *x,         [ X3 ]
            // i64 incx         [ X4 ]
        )
        {
            // clang-format off
            jmpn(0,end)
            jmpn(4,end)
            jmpz(4,end)

            //idjkj(0,1)          //n=n-1
            xkj(5,1)            //cmp=incx-1
            idjki(6,4,5)        //cmp=incx-1

            xkj(5,2)
            ipjkj(4,5)          //incx=incx*2

            jmpz(6,loop1)       //jmpz(loop1, cmp)
            //printf("execute loop2\n");
            jmp(loop2)          //jmp (loop2)
            pass()
            pass()
            

LABEL(loop1)xkj(5,0)
            rdjki(7,3,5)           //tmp1_re=*(pt)
            xkj(5,1)
            rdjki(8,3,5)        //tmp1_im=*(pt+1)

            fmul(9,7,1)         //tmp2_1=tmp1_re*a_re
            fmul(10,8,2)        //tmp2_2=tmp1_im*a_im
            fsub(11,9,10)        //res_re=tmp2_1-tmp2_2

            fmul(9,7,2)         //tmp2_1=tmp1_re*a_im
            fmul(10,8,1)        //tmp2_2=tmp1_im*a_re
            fadd(12,9,10)        //res_im=tmp2_1+tmp2_2

            sdjki(12,3,5)        //*(pt+1)=tmp1_im
            xkj(5,0)
            sdjki(11,3,5)        //*(pt)=tmp1_re

            isjkj(3,2)          //pt=pt+2
            idjkj(0,1)          //n=n-1
            pass()

            jmpp(0,loop1)       //jmpp (loop1, n)
            jmp(end)            //jmp (exit)

            

LABEL(loop2)xkj(5,0)
            rdjki(7,3,5)        //tmp1_re=*(pt)
            xkj(5,1)
            rdjki(8,3,5)        //tmp1_im=*(pt+1)

            fmul(9,7,1)
            fmul(10,8,2)
            fsub(11,9,10)

            fmul(9,7,2)
            fmul(10,8,1)
            fadd(12,9,10)

            sdjki(12,3,5)
            xkj(5,0)
            sdjki(11,3,5)

            isjki(3,3,4)          //pt=pt+incx
            idjkj(0,1)

            pass()
            jmpp(0,loop2)   
            jmp(end)

LABEL(end)  jmpk(15, 1)         // return to X15 (calling address) + 1
            // clang-format on
        }
    } // namespace BLAS
} // namespace CDC8600