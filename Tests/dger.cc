#include <blas/dger.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;
//test
extern "C" void dger_(i32*, i32*, f64*, f64*, i32*, f64*, i32*, f64*, i32*);

const int N = 20;

void test_dger(int count)
{
    reset();
    i32 m = rand() % 256;
    i32 n = rand() % 256;
    f64 alpha = 2.67;
    i32 incx = (rand() % 16 - 8) ;
    i32 incy = (rand() % 16 - 8) ;
    if (incx == 0) incx = 1;
    if (incy == 0) incy = 1;
    f64 *x = (f64*)CDC8600::memalloc( m * abs( incx ));
    f64 *y = (f64*)CDC8600::memalloc( n * abs( incy ));
    for (int i = 0; i < m *abs( incx ); i++) { x[i] = f64(drand48()); }
    for (int i = 0; i < n *abs( incy ); i++) { y[i] = f64(drand48()); }
    i32 lda = m;
    if(lda == 0) lda = 1;
    f64 *a = (f64*)CDC8600::memalloc(n*lda);
    f64 *A = new f64[n*lda];
    for (int i = 0; i < n*lda; i++) { A[i] = a[i]; }


    //tracing = false; if (n < 10) tracing = true;
    //tracing = true;

    // Call the dger function
    CDC8600::BLAS::dger(m, n, alpha, x, incx, y, incy, a, lda);

    dger_(&m, &n, &alpha, x, &incx, y, &incy, A, &lda);


    bool pass = true;
    for (int i = 0; i < n ; ++i) 
    {
        for (int j = 0; j < m ; ++j) 
        {

            if (a[i * m + j] != A[i * m + j]) 
            {
                pass = false; // Values don't match
            }
        }
    }

    delete [] A;

    cout << "dger [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", m = " << setw(3) << m;
    cout << ", incx = " << setw(2) << incx;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = " << setw(9) << instructions::count;
    cout << ", # of cycles = " << setw(9) << operations::maxcycle;
    cout << ") : ";    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    //cout << "completion:" << setw(8) << std::endl;

    //if (n < 2000)
     //dump(trace);
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dger(i);
    }
    return 0;
}
