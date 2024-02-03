#include <blas/drot.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t drot_(int32_t *, double *, int32_t *, double *, int32_t *, double *, double *);

const int N = 20;

void test_drot(int count)
{
    reset();

    // int32_t n = rand() % 256;
    int32_t n = rand() % 256;
    int32_t incx = (rand() % 16) - 8;
    int32_t incy = (rand() % 16) - 8;
    f64 angle = drand48() * (2 * M_PI);
    f64 c = cos(angle); // cos
    f64 s = sin(angle); // sin

    f64 *x = (f64*)CDC8600::memalloc(n*abs(incx));
    f64 *y = (f64*)CDC8600::memalloc(n*abs(incy));
    f64 *X = new f64[n*abs(incx)];
    f64 *Y = new f64[n*abs(incy)];

    for (int i = 0; i < n*abs(incx); i++) { X[i] = x[i] = drand48(); }
    for (int i = 0; i < n*abs(incy); i++) { Y[i] = y[i] = drand48(); }


    drot_(&n, X, &incx, Y, &incy, &c, &s);		// Reference implementation of DROT
    CDC8600::BLAS::drot(n, x, incx, y, incy, c, s);	// Implementation of DROT for the CDC8600

    bool pass = true;
    for (int i = 0; i < n*abs(incy); i++)
    {
        if (Y[i] != y[i])
        {
            cout << "Y[i]: " << Y[i] << "y[i]" << y[i] << endl;
            pass = false;
        }
    }
    for (int i = 0; i < n*abs(incx); i++)
    { 
        if (X[i] != x[i])
        {
            cout << "x: " << X[i] << ' ' << x[i] << endl;
            pass = false;
        }
    }

    delete [] X;
    delete [] Y;

    cout << "drot [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", incy = " << setw(2) << incy << ", c = " << setw(2) << c  << ", s = " << setw(2) << s << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_drot(i);
    }
    return 0;
}
