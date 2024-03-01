#include <blas/dtrmv_ltu.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t dtrmv_(char *, char *, char *, int32_t *, double *, int32_t *, double *, int32_t *);

const u32 N = 20;

void test_dtrmv_ltu(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 lda = n + rand() % 256;
    i32 incx = (rand() % 16) - 8; // incx = 0?
    incx = (incx == 0) ? 1:incx;
    char uplo = 'L';
    char trans = 'T';
    char diag = 'U';

    tracing = false; if (n < 10) tracing = true;

    int32_t nx = 1 + n * abs(incx);

    f64 *A = (f64*)CDC8600::memalloc(n*lda);
    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *X = new f64[nx];

    for (int i = 0; i < nx; i++) { X[i] = x[i] = drand48(); }
    for (int i = 0; i < n*lda; i++) { A[i] = drand48(); }

    dtrmv_(&uplo, &trans, &diag, &n, A, &lda, X, &incx);
    CDC8600::BLAS::dtrmv_ltu(n, A, lda, x, incx);

    bool pass = true;
    for (int i = 0; i < nx; i++)
    {
        if (x[i] != X[i])
        {
            pass = false;
        }
    }

    delete [] X;

    cout << "dtrmv_ltu [" << setw(2)<< count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", lda = " << setw(3) << lda;
    cout << ", incx = " << setw(2) << incx;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout <<  ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
    
    if (n < 10) dump(PROC[0].trace);


    return;
}

int main()
{
    for (u32 i = 0; i < N; i++)
    {
        test_dtrmv_ltu(i);
    }
    return 0;
}