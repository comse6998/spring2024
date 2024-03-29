#include <blas/dgemv_ta.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" int32_t dgemv_(char *trans, int32_t *m, int32_t *n, double *alpha, double *A, int32_t *LDA, double *x, int32_t *incx, double *beta, double *y, int32_t *incy);

const int N = 4;

void test_dgemv_ta(int count, bool tracing)
{
    reset();

    int32_t m = rand() % 256;
    int32_t n = rand() % 256;
    int32_t LDA = m + rand() % 256; if (0 == LDA) LDA = 1;
    //int32_t LDA = m;
    double alpha = f64(drand48());
    double beta = f64(drand48());
    int32_t incx = (rand() % 16) - 8; if (0 == incx) incx = 1;
    int32_t incy = (rand() % 16) - 8; if (0 == incy) incy = 1;
    //int32_t incx = 1;
    //int32_t incy = 1;
    
    if (tracing) {
      m = 3;
      n = 3;
      LDA = 8;
      incx = 1;
      incy = 1;
    }

    int32_t nx = m*abs(incx); if (0 == nx) nx = 1;
    int32_t ny = n*abs(incy); if (0 == ny) ny = 1;

    f64 *A = (f64*)CDC8600::memalloc(LDA*n);
    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *y = (f64*)CDC8600::memalloc(ny);
    f64 *Y = new f64[ny];

    for (int32_t i = 0; i < LDA*n; i++) { A[i] = f64(drand48()); }
    for (int32_t i = 0; i < nx; i++) { x[i] = f64(drand48()); }
    for (int32_t i = 0; i < ny; i++) { y[i] = f64(drand48()); }
    for (int32_t i = 0; i < ny; i++) { Y[i] = y[i]; }

    char trans = 'T';

    dgemv_(&trans, &m, &n, &alpha, A, &LDA, x, &incx, &beta, Y, &incy);         // Reference implementation of DAXPY
    CDC8600::BLAS::dgemv_ta(m, n, alpha, A, LDA, x, incx, beta, y, incy);       // Implementation of DAXPY for the CDC8600
    
    const double epsilon = 1e-9;
    bool pass = true;
    for (int i = 0; i < ny; i++)
    {
        
        if (abs(Y[i] - y[i]) > 
        ((abs(Y[i]) < abs(y[i]) ? abs(Y[i]) : abs(y[i])) + epsilon) * epsilon)
        {
            pass = false;
        }
    }

    delete [] Y;

    cout << "dgemv_ta [" << setw(2) << count << "] ";
    cout << "(m = " << setw(3) << m;
    cout << ", n = " << setw(3) << n;
    //cout << ", alpha = " << setw(2) << alpha;
    //cout << ", LDA = " << setw(2) << LDA;
    //cout << ", incx = " << setw(2) << incx;
    //cout << ", beta = " << setw(2) << beta;
    //cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
    cout << ", # of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
    cout << ") : ";

    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (tracing) {
      cout << "\n\n PROC[0] trace ========================== \n\n";
      dump(PROC[0].trace);
      cout << "\n\n PROC[1] trace ========================== \n\n";
      dump(PROC[1].trace);
      cout << "\n\n PROC[2] trace ========================== \n\n";
      dump(PROC[2].trace);
      cout << "\n\n PROC[3] trace ========================== \n\n";
      dump(PROC[3].trace);
      cout << "\n\n ========================== \n\n";
      dump(PROC[0].trace, "dgemv_ta.tr.0");
      cout << "\n\n ++++++++++++++++++++++++++ \n\n";
      dump(PROC[1].trace, "dgemv_ta.tr.1");
      cout << "\n\n ++++++++++++++++++++++++++ \n\n";
      dump(PROC[2].trace, "dgemv_ta.tr.2");
      cout << "\n\n ++++++++++++++++++++++++++ \n\n";
      dump(PROC[3].trace, "dgemv_ta.tr.3");
      cout << "\n\n ++++++++++++++++++++++++++ \n\n";
    }
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dgemv_ta(i, false);
    }

    test_dgemv_ta(0, true);
    return 0;
}
