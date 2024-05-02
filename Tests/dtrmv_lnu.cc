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

void test_dtrmv_lnu(int count, bool traceon, i32 n, i32 incx)
{
    reset();
    if (incx == 0) incx = 1;
    i32 lda = n + rand()%100;
    
    u32 nx = n*abs(incx); if (0 == nx) nx = 1;
    u32 na = n*lda;
    tracing = traceon;

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

    cout << "dtrmv_lnu [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", # of instr  = ("; cout << setw(9) << PROC[0].instr_count; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].instr_count; cout << ")";
    cout << ", # of ops    = ("; cout << setw(9) << PROC[0].op_count   ; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].op_count   ; cout << ")";
    cout << ", # of cycles = ("; cout << setw(9) << PROC[0].op_maxcycle; for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].op_maxcycle; cout << ")";
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (traceon) dump(PROC[0].trace, "dtrmv_lnu.tr.0");
    if (traceon) dump(PROC[1].trace, "dtrmv_lnu.tr.1");
    if (traceon) dump(PROC[2].trace, "dtrmv_lnu.tr.2");
    if (traceon) dump(PROC[3].trace, "dtrmv_lnu.tr.3");

}


int main(
    int         argc,
    char        **argv
)
{
    if(argc == 1)
    {
        for (u32 i = 0; i < N; i++)
        {
            i32 n = rand() % 100;
            i32 incx = (rand() % 16 - 8) ;
            test_dtrmv_lnu(i, false, n, incx);
        }
    }
    else if(argc == 3)
    {
        i32 n = atoi(argv[1]);
        i32 incx = atoi(argv[2]);
        test_dtrmv_lnu(0,true, n, incx);
    }
    else
    {
        cerr << "Usage : " << argv[0] << " [n incx]" << endl;
        return -1;
    }
    return 0;
}