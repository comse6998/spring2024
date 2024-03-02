#include <blas/dtrmv_unu.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" i32 dtrmv_(char *, char *, char *, i32 *, f64 *, i32 *, f64 *, i32 *);

const int N = 20;
const double EPSILON = 1e-9;

char UPLO = 'U';
char TRANS = 'N';
char DIAG = 'U';

void test_dtrmv_unu(int count)
{
    reset();
    tracing = false;

    i32 n = rand() % 64;
    i32 lda = n + rand() % 64;
    i32 incx = (rand() % 16) - 8; if (incx == 0) incx = 1;

    f64 *A = (f64*)CDC8600::memalloc(n*lda);
    f64 *tmp1 = (f64*)CDC8600::memalloc(2*n*abs(incx));
    f64 *X = tmp1 + n*abs(incx);
    f64 *tmp = new f64[2*n*abs(incx)];
    f64 *Y = tmp + n*abs(incx);

    for (int i = 0; i < n*lda; i++) { A[i] = f64(drand48()); }
    for (int i = -n*abs(incx); i < n*abs(incx); i++) { X[i] = Y[i] = f64(drand48()); }

    dtrmv_(&UPLO, &TRANS, &DIAG, &n, A, &lda, Y, &incx);
    CDC8600::BLAS::dtrmv_unu(n, A, lda, X, incx);  

    bool pass = true;
    
    int tcnt = 0;

    for (int i = -n*abs(incx); i < n*abs(incx); i+=abs(incx))
    {   
        if (abs(X[i] - Y[i]) > (min(abs(X[i]), abs(Y[i])) + EPSILON) * EPSILON)
        {
            pass = false;
        }
        tcnt++;
    }

    Y = NULL;
    delete [] tmp;
    tmp = NULL;

    cout << "dtrmv_unu [" << setw(2) << count << "] ";
    cout << "(lda = " << setw(3) << lda;
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", # of instr = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(6) << PROC[p].instr_count;
    cout << ", # of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(6) << PROC[p].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << endl;
    else
        cout << "FAIL" << endl;

    //if (n < 10) dump(PROC[0].trace);
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dtrmv_unu(i);
    }
    return 0;
}

