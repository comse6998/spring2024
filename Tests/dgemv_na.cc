#include <blas/dgemv_na.hh>
 #include <cmath>
 #include <cstdint>
 #include <cstdlib>
 #include <iostream>
 #include <iomanip>
 #include <complex>
 using namespace CDC8600;
 extern "C" void dgemv_(char*, i32*, i32*, f64*, f64*, i32*, f64*, i32*, f64*, f64*, i32*);

 const int N = 20;

 #define NUM_OF_PROC 4

 void test_dgemv_na(int count, i32 m, i32 n, f64 alpha, f64 beta, i32 incx, i32 incy, int traceon)
 {
    reset();
    f64 eps = 0.00000001;
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
     tracing = traceon;

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

    cout << "dgemv_na [" << setw(2) << count << "] ";
    cout << "(m = " << setw(3) << m;
    cout << ", n = " << setw(3) << n;
    cout << ", incx = " << setw(3) << incx;
    cout << ", incy = " << setw(3) << incy;
    cout << ", alpha = " << setw(3) << alpha;
    cout << ", beta = " << setw(3) << beta;
    cout << ", # of instr  = ("; cout << setw(9) << PROC[0].instr_count; 
    for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].instr_count; cout << ")";
    cout << ", # of ops    = ("; cout << setw(9) << PROC[0].op_count   ; 
    for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].op_count   ; cout << ")";
    cout << ", # of cycles = ("; cout << setw(9) << PROC[0].op_maxcycle; 
    for (u32 i = 1; i < params::Proc::N; i++) cout << ", " << setw(9) << PROC[i].op_maxcycle; cout << ")";
    cout << ") : ";
    if (pass){
        cout << "PASS" << std::endl;
    }
    else{
        cout << "FAIL" << std::endl;
    }
    if (traceon){
        dump(PROC[0].trace, "dgemv_na.tr.0");
        dump(PROC[1].trace, "dgemv_na.tr.1");
        dump(PROC[2].trace, "dgemv_na.tr.2");
        dump(PROC[3].trace, "dgemv_na.tr.3");
    }
}

 int main( 
    int		argc,
    char	**argv
    )
 {
    if (1 == argc)
    {
	for (u32 i = 0; i < N; i++)
	{
	    i32 m = rand()%256;
        i32 n = rand()%256;
        f64 alpha = (float)rand()/(float)(RAND_MAX/1);
        f64 beta = (float)rand()/(float)(RAND_MAX/1);
        i32 incx = (rand() % 16) -4;
        i32 incy = (rand() % 16 ) -4;
        int traceon = false;
	    test_dgemv_na(i, m, n, alpha, beta, incx, incy, traceon);
	}
    }
    else if (7 == argc)
    {
	i32 m = atoi(argv[1]);
	i32 n = atoi(argv[2]);
	i32 incx = atoi(argv[3]);
	i32 incy = atoi(argv[4]);
    f64 alpha = atof(argv[5]);
    f64 beta = atof(argv[6]);
	test_dgemv_na(0, m, n, alpha, beta, incx, incy, true);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [m n incx incy alpha beta]" << endl;
	return -1;
    }
    return 0;
 }