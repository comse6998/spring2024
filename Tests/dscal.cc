#include <blas/dscal.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

// extern "C" int32_t dscal_(uint64_t *, double *, double *, int64_t *);
extern "C" int32_t dscal_(int32_t *, double *, double *, int32_t *);

const int N = 20;

void test_dscal(int count, bool traceon, i32 n, i32 incx)
{
    reset();

    double a = drand48();
    
    tracing = traceon; 
    if (n < 40) tracing = true;

    f64 *x = (f64*)CDC8600::memalloc(n*abs(incx)); //Fortran input variable
    f64 *xX = (f64*)CDC8600::memalloc(n*abs(incx));//Fortran input variable

    for (i64 i = 0; i < n*abs(incx); i+=abs(incx)) 
    { 
        x[i] = drand48(); 
        xX[i] = x[i]; 
    }

    dscal_(&n, &a, xX, &incx);		// Reference implementation of DSCAL
    CDC8600::BLAS::dscal(n, a, x, incx);	// Implementation of DSCAL for the CDC8600

    bool pass = true;
    for (i64 i = 0; i < n*abs(incx); i+=abs(incx))
    {
        if (xX[i] != x[i])
        {
            pass = false;
        }

    }
    cout << "dscal [" << setw(2) << count << "] ";
    cout << "(n = " << setw(3) << n;
    cout << ", incx = " << setw(2) << incx;
    cout << ", a = " << setw(11) << a;
    cout << ", # of instr = " << setw(5) << PROC[0].instr_count;
    cout << ", # of cycles = " << setw(6) << PROC[0].op_maxcycle;
    cout << ") : ";

    // cout << "dscal [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", a = " << setw(12) << a << ", # of instr = " << setw(5) << instructions::count << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
    
    if (traceon)
    {
        dump(PROC[0].trace);
        dump(PROC[0].trace, "dscal.tr");
    }
}

//int main()
//{
//    for (int i = 0; i < N; i++)
//    {
//        test_dscal(i);
//    }
//    return 0;
//}

int main
(
    int		argc,
    char	**argv
)
{
    if (1 == argc)
    {
	for (int i = 0; i < N; i++)
	{
	    test_dscal(i, false, rand() % 256, (rand() % 16) - 8);
	}
    }
    else if (4 == argc)
    {
	i32 n = atoi(argv[1]);
	i32 incx = atoi(argv[2]);
	test_dscal(0, true, n, incx);
    }
    else
    {
	cerr << "Usage : " << argv[0] << " [n incx]" << endl;
	return -1;
    }
    return 0;
}
