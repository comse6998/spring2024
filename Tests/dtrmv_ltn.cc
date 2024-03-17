#include <blas/dtrmv_ltn.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" i32 dtrmv_(char *, char *, char *, i32 *, f64 *, i32 *, f64 *, i32 *);

// set the value of iteration and the error 
const int N = 5;
const double Epsilon = 1e-9;

// set the prameter
char UPLO = 'L'; char TRANS = 'T'; char DIAG = 'N';
void test_dtrmv_ltn(int count)
{
    reset();
    tracing = false;
    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    if (incx == 0) incx = 1;
    i32 LDA = n + rand()%256;
    
    u32 DX = n*abs(incx); if (0 == DX) DX = 1; // Dimension of X
    u32 nA = n*LDA;
    f64 *A = (f64*)CDC8600::memalloc(nA);
    f64 *X = (f64*)CDC8600::memalloc(DX);
    f64 *x = new f64[DX];
    for (u32 i = 0; i < nA; i++) { A[i] = drand48();}
    for(u32 i = 0;i<DX;i++){X[i] = x[i] = drand48();}
    dtrmv_(&UPLO, &TRANS, &DIAG, &n, A, &LDA, x, &incx);
    CDC8600::BLAS::dtrmv_ltn(n,A,LDA,X,incx);
    bool Pass = true;
    for(u32 i = 0;i<DX;i++)
    {
        if(abs(X[i] - x[i]) > (min(abs(X[i]), abs(x[i])) + Epsilon) * Epsilon)
       {
            Pass = false;
            break;
        }
    }
    delete [] x;
    cout << "dtrmv_ltn [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", LDA = " << setw(3) << LDA;
    cout << ", # of processors = " << params::Proc::N << ")" << std::endl;
    cout << "Cache Setting ( Line size = " << params::L1::linesize << ", nsets = "<< params::L1::nsets ;
    cout << ", nways = "<< params::L1::nways << ", latency = " << params::L1::latency << ")" << std::endl;

    cout << "# of instr  = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
    cout << '\n';
    cout << "# of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
    cout << " : ";
    if (Pass)
        cout << "PASS" << std::endl;
    else
  
        cout << "FAIL" << std::endl;
    
}
int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dtrmv_ltn(i);
    }
    return 0;
}