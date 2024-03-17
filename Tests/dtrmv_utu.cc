#include <blas/dtrmv_utu.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>


// DTRMV_UTU (upper/transpose/unit)
// A -> upper triangle of A
// A -> diagonal = 1
// x := A**T*x


using namespace CDC8600;

extern "C" i32 dtrmv_(char *, char *, char *, i32 *, f64 *, i32 *, f64 *, i32 *);

const int N = 20;
const double error = 1e-9;

void test_dtrmv_utu(int count)
{
    reset(); 

    i32 incx = (rand() % 16) - 8; 
    incx = (incx == 0) ? 1 : incx;
    i32 n = rand() % 256;
    i32 lda = n + rand() % 256;
    char UPLO = 'U'; 
    char TRANS = 'T'; 
    char DIAG = 'U';
    f64 *a = (f64*)CDC8600::memalloc(n*lda);
    f64 *x = (f64*)CDC8600::memalloc(n*abs(incx));  
    f64 *y = new f64[n*abs(incx)];                
    
    for (int i = 0; i < n*lda; i++) {
        a[i] = drand48();
         }
    for (int i = 0; i < n*abs(incx); i++) {
        x[i] = y[i] = drand48(); 
         }
    dtrmv_(&UPLO, &TRANS, &DIAG, &n, a, &lda, y, &incx);     // Reference implementation of dtrmv_utu
    CDC8600::BLAS::dtrmv_utu(n, a, lda, x, incx);            // Implementation of dtrmv_utu for the CDC8600


    
    bool pass = true;
    for (int i = 0; i < n*abs(incx); i++)
    {
        if (abs(x[i] - y[i]) > (min(abs(x[i]), abs(y[i])) + error) * error)
        {
            pass = false;
        }
    }

    delete [] y;

    cout << "dtrmv_utu [" << setw(2) << count << "] ";
    cout << "(lda = " << setw(3) << lda;
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", # of instr = ";
    for (u32 i = 0; i < params::Proc::N; i++)cout << setw(9) << PROC[i].instr_count;
    cout << ", # of cycles = ";
    for (u32 i = 0; i < params::Proc::N; i++)cout << setw(9) << PROC[i].op_maxcycle;
    
    cout << ") : ";

    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dtrmv_utu(i);
    }
    return 0;
}
