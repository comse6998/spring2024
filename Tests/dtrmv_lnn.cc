#include <blas/dtrmv_lnn.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace CDC8600;

extern "C" i32 dtrmv_(char *, char *, char *, i32 *, f64 *, i32 *, f64 *, i32 *);

const int N = 20;
const double epsilon = 1e-9;

void test_dtrmv_lnn(int count)
{
    reset();

    i32 n = rand() % 256;
    i32 lda = n + rand() % 256;
    i32 incx = (rand() % 16); if (incx == 0) incx = 1;
    char uplo = 'L', trans = 'N', diag = 'N';
    bool pass = true;

    tracing = false;

    f64 *A = (f64*)CDC8600::memalloc(n*lda);
    f64 *X = (f64*)CDC8600::memalloc(n*abs(incx));
    f64 *Y = new f64[n*abs(incx)];

    for (int i = 0; i < n*lda; i++) { A[i] = (f64)rand(); }
    for (int i = 0; i < n*abs(incx); i++) { X[i] = Y[i] = (f64)rand(); }

    /* Compare dtrmv_lnn to the established version. */
    dtrmv_(&uplo, &trans, &diag, &n, A, &lda, Y, &incx);
    CDC8600::BLAS::dtrmv_lnn(n, A, lda, X, incx);

    for (int i = 0; i < n*abs(incx); i+=incx)
    {
        if (abs(X[i] - Y[i]) > (min(abs(X[i]), abs(Y[i])) + epsilon) * epsilon)
        {
            pass = false;
        }
    }

    delete [] Y;

    cout << "dtrmv_lnn [" << setw(2) << count << "] ";
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

    if (n < 10) dump(PROC[0].trace);
}

int main()
{
    //srand(time(NULL));   // For testing.
    for (int i = 0; i < N; i++)
    {
        test_dtrmv_lnn(i);
    }
    return 0;
}
