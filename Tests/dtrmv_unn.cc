#include <blas/dtrmv_unn.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" i32 dtrmv_(char *, char *, char *, i32 *, f64 *, i32 *, f64 *, i32 *);

const int N = 20;
const double EPSILON = 1e-9;

void test_dtrmv_unn(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 lda = n + rand() % 256;
    i32 incx = (rand() % 16) - 8; if (incx == 0) incx = 1;
    char uplo = 'U', trans = 'N', diag = 'N';

    tracing = false;

    f64 *A = (f64*)CDC8600::memalloc(n*lda);
    f64 *X = (f64*)CDC8600::memalloc(n*abs(incx));    // Input matrix
    f64 *Y = new f64[n*abs(incx)];                    // Reference matrix

    for (int i = 0; i < n*lda; i++) { A[i] = f64(drand48()); }
    for (int i = 0; i < n*abs(incx); i++) { X[i] = Y[i] = f64(drand48()); }

    dtrmv_(&uplo, &trans, &diag, &n, A, &lda, Y, &incx);     // Reference implementation of dtrmv_unn
    CDC8600::BLAS::dtrmv_unn(n, A, lda, X, incx);            // Implementation of dtrmv_unn for the CDC8600

    bool pass = true;
    for (int i = 0; i < n*abs(incx); i++)
    {
        if (abs(X[i] - Y[i]) > (min(abs(X[i]), abs(Y[i])) + EPSILON) * EPSILON)
        {
            pass = false;
        }
    }

    delete [] Y;
    CDC8600::memfree(X, n*abs(incx));
    CDC8600::memfree(A, n*lda);

    cout << "dtrmv_unn [" << setw(2) << count << "] ";
    cout << "(lda = " << setw(3) << lda;
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", # of instr = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
    cout << ", # of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (n < 10) {
        for (u32 p = 0; p < params::Proc::N; p++) {
            cout << "PROC[" << p << "], # of instr = " << PROC[p].instr_count << endl;
            dump(PROC[p].trace);
            dump(PROC[p].trace, ("dtrmv_unn.tr." + to_string(p)).c_str());
        }
    }
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dtrmv_unn(i);
    }
    return 0;
}
