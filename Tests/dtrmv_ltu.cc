#include <blas/dtrmv_ltu.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t dtrmv_(char *, char *, char *, int32_t *, double *, int32_t *, double *, int32_t *);

const u32 N = 20;
const double EPSILON = 1e-9;

void test_dtrmv_ltu(int count, bool traceon, i32 n, i32 lda, i32 incx)
{
    reset();

    tracing = traceon;

    char uplo = 'L';
    char trans = 'T';
    char diag = 'U';

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
        if(!(abs(X[i] - x[i]) < (min(abs(X[i]), abs(x[i])) + EPSILON) * EPSILON))
        {
            pass = false;
            break;
        }
    }

    delete [] X;

    if (traceon) {
        for (u32 i = 0; i < params::Proc::N; i++) {
            // dump(PROC[i].trace); // print to stdout
            dump(PROC[i].trace, ("dtrmv_ltu.tr." + std::to_string(i)).c_str());
        }
    }

    cout << "dtrmv_ltu [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    // cout << ", lda = " << setw(3) << lda;
    cout << ", incx = " << setw(3) << incx;
    cout << ", # of instr  = ("; cout << setw(9) << PROC[0].instr_count; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].instr_count; cout << ")";
    cout << ", # of ops    = ("; cout << setw(9) << PROC[0].op_count   ; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].op_count   ; cout << ")";
    cout << ", # of cycles = ("; cout << setw(9) << PROC[0].op_maxcycle; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].op_maxcycle; cout << ")";
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
    
    CDC8600::memfree(x, nx);
    CDC8600::memfree(A, n*lda);
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        for (u32 i = 0; i < N; i++)
        {
            i32 n = rand() % 256;
            i32 lda = n + rand() % 256;
            lda = max(lda, 1);
            i32 incx = (rand() % 16) - 8; // incx = 0?
            incx = (incx == 0) ? 1:incx;
            test_dtrmv_ltu(i, false, n, lda, incx);
        }
    } else if (argc == 3) {
        i32 n = atoi(argv[1]);
        i32 incx = atoi(argv[2]);
        if (incx == 0) {
            cerr << "incx cannot be 0" << endl;
            exit(1);
        }
        test_dtrmv_ltu(0, true, n, n + 1, incx); // lda is default to be n + 1

    } else {
        cerr << "Usage : " << argv[0] << " [n incx]" << endl;
	    exit(1);
    }
    return 0;
}