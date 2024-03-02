#include <blas/dgemv_na.hh>
 #include <cmath>
 #include <cstdint>
 #include <cstdlib>
 #include <iostream>
 #include <iomanip>
 #include <complex>

 using namespace CDC8600;
 extern "C" void dgemv_(char*, i32*, i32*, f64*, f64*, i32*, f64*, i32*, f64*, f64*, i32*);

 const int N = 50;

 void test_dgemv(int count)
 {
    reset();
    f64 eps = 0.5;
    i32 m = rand() % 256;
    i32 n = rand() % 256;
    f64 alpha = (float)rand()/(float)(RAND_MAX/1);
    f64 beta = (float)rand()/(float)(RAND_MAX/1);
    i32 incx = (rand() % 16) -4;
    i32 incy = (rand() % 16 ) -4;
    if (incx == 0) incx = 1;
    if (incy == 0) incy = 1;
    f64 *x = (f64*)CDC8600::memalloc( n * abs( incx ));
    f64 *y = (f64*)CDC8600::memalloc( m * abs( incy ));
    f64 *y_ = (f64*)CDC8600::memalloc( m * abs( incy ));
    f64 *A = (f64*)CDC8600::memalloc( m * n);
    for (int i = 0; i < n *abs( incx ); i++) { x[i] = f64(drand48()); }
    for (int i = 0; i < m *abs( incy ); i++) { y[i] = f64(drand48()); }
    for (int i = 0; i < m *abs( incy ); i++) { y_[i] = y[i]; }
    for (int i = 0; i < m *n; i++) { A[i] = f64(drand48()); }
    i32 lda = m;
    if(lda == 0) lda = 1;
    
    char trans = 'N';
     //tracing = false; if (n < 10) tracing = true;
     //tracing = true;

     // Call the dger function
    dgemv_(&trans, &m, &n, &alpha, A,&lda,x, &incx, &beta, y_, &incy);

    CDC8600::BLAS::dgemv_na(m, n, alpha, A, lda, x, incx, beta, y, incy);

    i32 iy = 0;
    if (incy <= 0) iy = (-m+1)*incy;

     bool pass = true;
     for (int i = 0; i < m; ++i) 
     {
             if (abs(y[iy] - y_[iy]) > eps) 
             {
                 pass = false; // Values don't match
             }
             iy += incy;
     }

     //delete [] A;

     cout << "dgmev_na [" << setw(2) << count << "] ";
     cout << "(n = " << setw(3) << n;
     cout << ", m = " << setw(3) << m;
     cout << ", incx = " << setw(2) << incx;
     cout << ", incy = " << setw(2) << incy;
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
        test_dgemv(i);
     }
     return 0;
 }