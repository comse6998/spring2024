#include <blas/drot.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t drot_(int32_t *, double *, int32_t *, double *, int32_t *, double *, double *);

const int N = 25;

void test_drot(int count, bool traceon, i32 n, i32 incx, i32 incy)
{
    reset();

    f64 angle = drand48() * (2 * M_PI);
    f64 c = cos(angle); // cos
    f64 s = sin(angle); // sin
    int32_t nx = 1 + n * abs(incx); // not n * abs(incx)
    int32_t ny = 1 + n * abs(incy); // not n * abs(incy)

    tracing = traceon;

    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *y = (f64*)CDC8600::memalloc(ny);
    f64 *X = new f64[nx];
    f64 *Y = new f64[ny];

    for (int i = 0; i < nx; i++) { X[i] = x[i] = drand48(); }
    for (int i = 0; i < ny; i++) { Y[i] = y[i] = drand48(); }


    drot_(&n, X, &incx, Y, &incy, &c, &s);		// Reference implementation of DROT
    CDC8600::BLAS::drot(n, x, incx, y, incy, c, s);	// Implementation of DROT for the CDC8600

    bool pass = true;
    for (int i = 0; i < ny; i++)
    {
        if (Y[i] != y[i])
        {
            pass = false;
        }
    }
    for (int i = 0; i < nx; i++)
    { 
        if (X[i] != x[i])
        {
            pass = false;
        }
    }

    delete [] X;
    delete [] Y;
    CDC8600::memfree(y, ny);
    CDC8600::memfree(x, nx);

    if (tracing) {
        dump(PROC[0].trace);
        dump(PROC[0].trace, "drot.tr");
    }

    cout << "drot [" << setw(2)<< count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", incy = " << setw(2) << incy;
    cout << ", c = " << setw(2) << c << ", s = " << setw(2) << s;
    cout << ", # of instr = " << setw(9) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout <<  ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
    

}

int main(int argc, char **argv)
{
    if (argc == 1) {
        for (int i = 0; i < N; i++)
        {
            i32 n = rand() % 256;
            i32 incx = (rand() % 16) - 8;
            i32 incy = (rand() % 16) - 8;
            test_drot(i, false, n, incx, incy);
        }
    } else if (argc == 4) {
        i32 n = atoi(argv[1]);
        i32 incx = atoi(argv[2]);
        i32 incy = atoi(argv[3]);
        test_drot(0, true, n, incx, incy);
    } else {
        cerr << "Usage : " << argv[0] << " [n incx incy]" << endl;
        return -1;
    }
    return 0;
}
