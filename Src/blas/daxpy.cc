#include <CDC8600.hh>
#include <ISA.hh>
#include <blas/daxpy.hh>

namespace CDC8600
{
        namespace BLAS
        {
                void daxpy(u64 n, f64 a, f64 *x, i64 incx, f64 *y, i64 incy)
                {
                        Call(daxpy_asm)(n, a, x, incx, y, incy);
                }

                void daxpy_cpp(u64 n, f64 a, f64 *x, i64 incx, f64 *y, i64 incy)
                {
                        if (n <= 0) return; // if n <= 0, return
                        if (a == 0) return; // IF (DA.EQ.0.0d0) RETURN
                        if (incx == 1 && incy == 1) {
                                i64 M = n%4;
                                if (M != 0 ) {
                                        for (int i = 0; i < M; i++)
                                        y[i] += a * x[i];
                                }

                            if (n < 4) return;
                    
                            for (u32 i = M; i < n; i+=4){
                                y[i] += a * x[i];
                                y[i+1] += a * x[i+1];
                                y[i+2] += a * x[i+2];
                                y[i+3] += a * x[i+3];
                            }
                        } else {
                                i64 ix = 0;                             // First element of x
                                i64 iy = 0;                             // First element of y
                                if (incx < 0) ix = (-n+1)*incx; // If incx < 0, start in the correct position with inverse iteration
                                if (incy < 0) iy = (-n+1)*incy; // If incy < 0, start in the correct position with inverse iteration
                                for (u32 i = 0; i < n; i++){
                                        y[iy] += a * x[ix];                     // DY(IY) = DY(IY) + DA*DX(IX)
                                        ix += incx;                     // Next element of x
                                        iy += incy;                     // Next element of y
                                }
                        }
                }

                void daxpy_asm
                (
                        // u64 n,           [ X0 ]
                        // f64 a            [ X1 ]
                        // f64 *x,          [ X2 ]
                        // i64 incx,        [ X3 ]
                        // f64 *y,          [ X4 ]
                        // i64 incy         [ X5 ]
                )
                {
                        jmpn(0, end)        // if n <= 0 goto end
                        jmpz(0, end)        
                        
            jmpz(1, end)        // if a == 0 goto end
            // check if incx == 0
                        xkj(6, 0)           // X6 (tmp) = 0
                        idjkj(6, 1)         // X6 (tmp) = -1
                        isjki(6, 6, 3)      // X6 (tmp) = X6 (-1) + X4 (incx)
                        pass()
                        pass()
                        pass()
                        jmpnz(6, Common)   // if X4 (incx) != 1 goto general
                        idjkj(7, 1)         // X7 (tmp) = -1
                        isjki(7, 7, 5)      // X7 (tmp) = X7 (-1) + X5 (incy)
                        jmpnz(7, Common)   // if X6 (incy) != 1 goto general

                        
                        //idjkj(0, 1)         // X0 (n) = X0 (n) - 1

            rdjk(6, 0)           // initiate M = 0
            rdjk(7, 0)           // the tmp number of M, initial as n


LABEL(MOD) rdjk(6, 7)          // assign tem M to M
            rdjk(8, 7)          // another tem M[10] for M
            idjkj(7, 4)         // tem - 4 = tmp
                        pass()
                        pass()
                        pass()
            jmpn(7, Loop_M)     // if tem < 0, M != 0, do the loop
            jmpz(7, equal_0)        //when M = 0, move to loop chunk_4
            jmp(MOD)
                        
                        

LABEL(Loop_M) jmpz(6, Loop_chunk4)  // when M is 0, jump to Loop_chunk4
            
            idjkj(6, 1)             // M = M - 1
                        idjkj(0, 1)                             // X[0] = X[0] - 1
            rdjki(10, 2, 6)                     // X[tmp1] = MEM[X[2] + X[6]]
                        rdjki(11, 4, 6)                 // X[tmp2] = MEM[X[4](y) + X[6](M)]
                        fmul(10,10,1)                   // X[tmp1] = X[10](x) * a
                        fadd(11, 10, 11)                // X[tmp2] = X[tmp1] + X[tmp2]
                        sdjki(11, 4, 6)                 // MEM[X[4] + X[6](M)] = X[tmp2]
                        pass()
                        jmp(Loop_M)

LABEL(equal_0) xkj(8, 0)                        // when n mod 4 == 0, we could directly deal with this problem
                                pass()
                                pass()
                                pass()
                                

LABEL(Loop_chunk4) jmpz(0, end)                 // if n<4 return or complete the loop
                        idjkj(0, 4)                             // // X[0] = X[0] - 4
                        rdjki(9, 4, 8)      // X9 (tmp1) = MEM[X4 (y) + X8 (M)]
                        rdjki(10, 2, 8)     // X10 (tmp2) = MEM[X3 (x) + X8 (M)]
                        fmul(10, 10, 1)     // X10 (tmp2) = X[10] * X[1](a)
                        fadd(9, 9, 10)      // X9 (tmp1) = X[9] + X[10]
                        sdjki(9, 4, 8)      // MEM[X4 (y) + X8 (M)]= X9 (tmp1)

                        isjkj(8, 1)                     // M = M+1
                        rdjki(9, 4, 8)      // X9 (tmp1) = MEM[X4 (y) + X8 (M)]
                        rdjki(10, 2, 8)     // X10 (tmp2) = MEM[X3 (x) + X8 (M)]
                        fmul(10, 10, 1)     // X10 (tmp2) = X[10] * X[1](a)
                        fadd(9, 9, 10)      // X9 (tmp1) = X[9] + X[10]
                        sdjki(9, 4, 8)      // MEM[X4 (y) + X8 (M)]= X9 (tmp1)

                        isjkj(8, 1)                     // M = M+1
                        rdjki(9, 4, 8)      // X9 (tmp1) = MEM[X4 (y) + X8 (M)]
                        rdjki(10, 2, 8)     // X10 (tmp2) = MEM[X3 (x) + X8 (M)]
                        fmul(10, 10, 1)     // X10 (tmp2) = X[10] * X[1](a)
                        fadd(9, 9, 10)      // X9 (tmp1) = X[9] + X[10]
                        sdjki(9, 4, 8)      // MEM[X4 (y) + X8 (M)]= X9 (tmp1)

                        isjkj(8, 1)                     // M = M+1
                        rdjki(9, 4, 8)      // X9 (tmp1) = MEM[X4 (y) + X8 (M)]
                        rdjki(10, 2, 8)     // X10 (tmp2) = MEM[X3 (x) + X8 (M)]
                        fmul(10, 10, 1)     // X10 (tmp2) = X[10] * X[1](a)
                        fadd(9, 9, 10)      // X9 (tmp1) = X[9] + X[10]
                        sdjki(9, 4, 8)      // MEM[X4 (y) + X8 (M)]= X9 (tmp1)

                        isjkj(8, 1)                     // M = M+4 (next location)
                        pass()
                        jmp(Loop_chunk4)
                        pass()
                        pass()
                        

LABEL(Common) xkj(7, 0)           // X7 (ix) = 0
                        xkj(8, 0)           // X8 (iy) = 0
                        jmpp(3, L_Y)         // if X4 (incx) > 0 goto L_Y
                        idzkj(7, 0)         // X7 (ix) = -X0 (n)
                        isjkj(7, 1)         // X7 (ix) = X7(-n) + 1
                        ipjkj(7, 3)         // X7 (ix) = X7 (-n+1) * X3 (incx)
                        pass()


LABEL(L_Y)      jmpp(5, Main_loop)       // if X5 (incy) > 0 goto Main_loop
                        idzkj(8, 0)         // X8 (iy) = -X0 (n)
                        isjkj(8, 1)         // X8 (iy) = X8(-n) + 1
                        ipjkj(8, 5)         // X8 (iy) = X8 (-n+1) * X5 (incy)
                        pass()
			pass()
			pass()


LABEL(Main_loop) jmpz(0, end)        // if X0 (n) = 0 goto end
                        rdjki(9, 4, 8)      // X9 (tmp1) = MEM[X4 (y) + X8 (iy)]
                        rdjki(10, 2, 7)     // X10 (tmp2) = MEM[X3 (x) + X7 (ix)]
                        fmul(10, 10, 1)     // X10 (tmp2) = X[10] * X[1](a)
                        fadd(9, 9, 10)      // X9 (tmp1) = X[9] + X[10]
                        sdjki(9, 4, 8)      // MEM[X4 (y) + X8 (iy)]= X9 (tmp1)

                        isjki(7, 7, 3)      // X7 (ix) = X7 (ix) + X3 (incx)
                        isjki(8, 8, 5)      // X8 (iy) = X8 (iy) + X5 (incy)
                        idjkj(0, 1)         // X0 (n) = X0 (n) - 1
                        jmp(Main_loop)

LABEL(end)      jmpk(15, 1)         // return to X15 (calling address) + 1
                }
        } // namespace BLAS
} // namespace CDC8600
