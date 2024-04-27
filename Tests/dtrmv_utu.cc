#include <blas/dtrmv_utu.hh>
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

void test_dtrmv_utu(int count, bool traceon,i32 n, i32 lda, i32 incx)
{
    reset();
    char uplo = 'U', trans = 'T', diag = 'U';

    tracing = traceon;
    if (n < 10) tracing = true;
    f64 *A = (f64*)CDC8600::memalloc(n*lda);
    f64 *X = (f64*)CDC8600::memalloc(n*abs(incx));    // Input matrix
    f64 *Y = new f64[n*abs(incx)];                    // Reference matrix

    for (int i = 0; i < n*lda; i++) { A[i] = f64(drand48()); }
    for (int i = 0; i < n*abs(incx); i++) { X[i] = Y[i] = f64(drand48()); }

    dtrmv_(&uplo, &trans, &diag, &n, A, &lda, Y, &incx);     
    CDC8600::BLAS::dtrmv_utu(n, A, lda, X, incx);            

    bool pass = true;
    for (int i = 0; i < n*abs(incx); i++)
    {
        if (abs(X[i] - Y[i]) > (min(abs(X[i]), abs(Y[i])) + EPSILON) * EPSILON)
        {
            pass = false;
        }
    }

    delete [] Y;

    cout << "dtrmv_utu [" << setw(2) << count << "] ";
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

    if (traceon) 
    {
        dump(PROC[0].trace, "dtrmv_utu0.tr");
        dump(PROC[1].trace, "dtrmv_utu1.tr");
        dump(PROC[2].trace, "dtrmv_utu2.tr");
        dump(PROC[3].trace, "dtrmv_utu3.tr");
    }
}

int main(
int		argc,
char	**argv)
{
    if (argc == 1)
    {
        for (int i = 0; i < N; i++)
        {
            i32 n = rand() % 256;
            i32 lda = n + rand() % 256;
            i32 incx = (rand() % 16) - 8; if (incx == 0) incx = 1;
            test_dtrmv_utu(i, false, n, lda, incx);
        }
    }
    else if(argc == 4)
    {
    	i32 n = atoi(argv[1]);
        i32 lda = atoi(argv[2]);
    	i32 incx = atoi(argv[3]);
        test_dtrmv_utu(0, true, n, lda, incx);

    }
    else
    {
	cerr << "Usage : " << argv[0] << " [n lda incx]" << endl;
	return -1;        
    }
    return 0;
}
