#include <blas/ddot.hh>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" f64 ddot_(i32*, f64 *, i32*, f64 *, i32*);

const int N = 20;

void test_ddot(int count, bool traceon, i32 n, i32 incx, i32 incy)
{
    reset();

    u32 dx = n*abs(incx); if (0 == dx) dx = 1;
    u32 dy = n*abs(incy); if (0 == dy) dy = 1;

    tracing = traceon;

    f64 *x = (f64*)CDC8600::memalloc(dx*2);
    f64 *y = (f64*)CDC8600::memalloc(dy*2);

    for (u32 i = 0; i < dx; i++) { x[i] = double(drand48()); }
    for (u32 i = 0; i < dy; i++) { y[i] = double(drand48()); }

    f64 ref_ = ddot_(&n, x, &incx, y, &incy);
    f64 new_ = CDC8600::BLAS::ddot(n, x, incx, y, incy);

    double epsilon = 1e-6;
    bool pass = true;

    if (abs(ref_ - new_) >
        ((abs(ref_) < abs(new_) ? abs(ref_) : abs(new_)) + epsilon) * epsilon)
    {
        pass = false;
    }

    CDC8600::memfree(y, dx*2);
    CDC8600::memfree(x, dy*2);

    cout << "ddot [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", incy = " << setw(2) << incy;
    cout << ", # of instr = "  << setw(9) << PROC[0].instr_count;
    cout << ", # of ops = "    << setw(9) << PROC[0].op_count;
    cout << ", # of cycles = " << setw(9) << PROC[0].op_maxcycle;
    cout << ") : ";
    if (pass)
        std::cout << "PASS" << std::endl;
    else
        std::cout << "FAIL" << std::endl;

    if (traceon) dump(PROC[0].trace);
    if (traceon) dump(PROC[0].trace, "ddot.tr");
}

int main
(
    int		argc,
    char	**argv
)
{
    if (1 == argc)
    {
	for (u32 i = 0; i < N; i++)
	{
	    i32 n = rand() % 256;
	    i32 incx = (rand() % 16) - 8;
	    i32 incy = (rand() % 16) - 8;
	    test_ddot(i, false, n, incx, incy);
	}
    }
    else if (4 == argc)
    {
	i32 n = atoi(argv[1]);
	i32 incx = atoi(argv[2]);
	i32 incy = atoi(argv[3]);
	test_ddot(0, true, n, incx, incy);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [n incx incy]" << endl;
	return -1;
    }
    return 0;
}
