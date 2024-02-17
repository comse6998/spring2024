#include <blas/zscal.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" void zscal_(i64*, c128*, c128*, i64*);

const int N = 20;

void test_zscal(int count)
{
    reset();

    i64 n = rand() % 256;
    //i64 n =1;
    i64 incx = (rand() % 16) - 8;
    //i64 incx =1;

    c128 a{drand48(),drand48()};

    uint32_t nx = n*abs(incx); if (0 == nx) nx = 1;


    tracing = false; if (n < 10) tracing = true;

    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *X = (c128*)CDC8600::memalloc(nx*2);
    //f64 *y = (f64*)CDC8600::memalloc(ny);
    //c128 *X = new c128[nx];

    for (int i = 0; i < nx; i++) 
        {
            x[i] = {drand48(),drand48()};
            X[i].real(x[i].real());
            X[i].imag(x[i].imag());
        }
    //for (int i = 0; i < ny; i++) { y[i] = 0.0;	 }
    //for (int i = 0; i < ny; i++) { Y[i] = 0.0;	 }

    zscal_(&n, &a, x, &incx);		// Reference implementation of DCOPY
    CDC8600::BLAS::zscal(n, a, X, incx);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    for (int i = 0; i < nx; i++)
    {
        if (X[i] != x[i])
        {
            pass = false;
            //printf("%lf+%lfi, %lf+%lfi\n",X[i].real(),X[i].imag(),x[i].real(),x[i].imag());
        }
    }

    //delete [] X;

    cout << "zscal [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", # of instr = " << setw(9) << instructions::count << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (n < 10) dump(trace);
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_zscal(i);
    }
    return 0;
}