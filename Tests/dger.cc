#include <blas/dger.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace CDC8600;

extern "C" void dger_(i32*, i32*, f64*, f64*, i32*, f64*, i32*, f64*, i32*);

const int N = 20;

void test_dger(int count, bool traceon, i32 n, i32 m, i32 incx, i32 incy, f64 alpha)
{
    reset();

    tracing = traceon;

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
    cout << "(m = " << setw(3) << m;
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", incy = " << setw(3) << incy;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ", # of instr = " << setw(9) << PROC[1].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[1].op_maxcycle;
    cout << ", # of instr = " << setw(9) << PROC[2].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[2].op_maxcycle;
    cout << ", # of instr = " << setw(9) << PROC[3].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[3].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (traceon) dump(PROC[0].trace, "dger.tr.0");
    if (traceon) dump(PROC[1].trace, "dger.tr.1");
    if (traceon) dump(PROC[2].trace, "dger.tr.2");
    if (traceon) dump(PROC[3].trace, "dger.tr.3");

}

int main
(
    int         argc,
    char        **argv
)
{
    if (1 == argc)
    {
        for (u32 i = 0; i < N; i++)
        {
            i32 m = rand() % 100;
            i32 n = rand() % 100;
            i32 incx = (rand() % 16 - 8) ;
            i32 incy = (rand() % 16 - 8) ;
            f64 alpha = drand48();
            test_dger(i, false, n, m, incx, incy, alpha);
        }
    }
    else if (6 == argc)
    {
            i32 n = atoi(argv[1]);
            i32 m = atoi(argv[2]);
            i32 incx = atoi(argv[3]);
            i32 incy = atoi(argv[4]);
            i32 alpha = atof(argv[5]);
            test_dger(0, true, n, m, incx, incy, alpha);
    }
    else
    {
        cerr << "Usage : " << argv[0] << " [n m incx incy alpha]" << endl;
        return -1;
    }
    return 0;
}

