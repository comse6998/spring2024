#include <blas/dtrmv_lnu.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" i32 dtrmv_(char *, char *, char *, i32 *, f64 *, i32 *, f64 *, i32 *);

const int N = 20;
const double epsilon = 1e-9;

char UPLO = 'L';
char TRANS = 'N';
char DIAG = 'U';

void test_dtrmv_lnu(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 incx = (rand() % 16) - 8;
    if (incx == 0) incx = 1;
    // how big should lda be than n ?? 
    i32 lda = n + rand()%256;
    
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 na = n*lda;
    tracing = false; 
    if (n < 10) tracing = true;

    f64 *A = (f64*)CDC8600::memalloc(na);
    f64 *X = (f64*)CDC8600::memalloc(nx);
    f64 *x = new f64[nx];

    for (u32 i = 0; i < na; i++) { A[i] = drand48();}
    for(u32 i = 0;i<nx;i++){X[i] = x[i] = drand48();}

    dtrmv_(&UPLO, &TRANS, &DIAG, &n, A, &lda, x, &incx);
    CDC8600::BLAS::dtrmv_lnu(n,A,lda,X,incx);

    bool pass = true;

    for(u32 i = 0;i<nx;i++)
    {
        if(!(abs(X[i] - x[i]) < (min(abs(X[i]), abs(x[i])) + epsilon) * epsilon ))
        {
            pass = false;
            break;
        }
    }

    delete [] x;

    cout << "dtrmv_lmu [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", lda = " << setw(3) << lda;
    cout << ", # of instr = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
    cout << ", # of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
    
    if (n < 10)
    {
        dump(PROC[0].trace, "dtrmv_lnu.tr.0");
        dump(PROC[1].trace, "dtrmv_lnu.tr.1");
        dump(PROC[2].trace, "dtrmv_lnu.tr.2");
        dump(PROC[3].trace, "dtrmv_lnu.tr.3");
    } 

}


int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dtrmv_lnu(i);
    }
    return 0;
}