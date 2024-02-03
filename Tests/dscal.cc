#include <blas/dscal.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

// void dscal(u64 n, f64 a, f64 *x, i64 incx);
extern "C" int32_t dscal_(u64 *, f64 *, f64 *, i64 *);
const int N = 20;

void test_dscal(int count)
{
    reset();

    u64 n = rand() % 8;
    f64 a = drand48();
    i64 incx = (rand() % 8);
    // i64 incx = 1;

    f64 *x = (f64*)CDC8600::memalloc(n*abs(incx));
    f64 *xX = (f64*)CDC8600::memalloc(n*abs(incx));

    for (i64 i = 0; i < n*abs(incx); i+=abs(incx)) 
    { 
        x[i] = drand48(); 
        xX[i] = x[i]; 
    }
    
    // void dscal(u64 n, f64 a, f64 *x, i64 incx);
    dscal_(&n, &a, xX, &incx);		// Reference implementation of DCOPY
    // dscal_(&n, x, &incx, Y, &incy);		// Reference implementation of DCOPY
    CDC8600::BLAS::dscal(n, a, x, incx);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    for (i64 i = 0; i < n*abs(incx); i+=abs(incx))
    {
		// cout << "OP\t\t "  << n  << " "<< a << " " << x[i] << " " << incx << " OP ENd"  << endl;
		// cout << "Fortran\t\t "  << n  << " "<< a << " " << xX[i] << " " << incx << " Fortran end"  << endl;
        if (xX[i] != x[i])
        {
            pass = false;
        }

    }

        cout << "n = "<< n << "\ta =" << a << "\t  \tincx =" << incx << " : " ;
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dscal(i);
    }
    return 0;
}
