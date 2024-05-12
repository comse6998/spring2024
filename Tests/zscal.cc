#include <blas/zscal.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

//IMPORTANT: FUNC_TEST is for demo only, comment these lines if not needed
#ifndef FUNC_TEST
#define FUNC_TEST
#endif

using namespace CDC8600;

extern "C" void zscal_(i64*, c128*, c128*, i64*);

const int N = 20;

void test_zscal(int count, bool traceon, i64 n, c128 a, i64 incx)
{
    reset();
    uint32_t nx = n*abs(incx); if (0 == nx) nx = 1;

    tracing = traceon; 
    c128 *x = (c128*)CDC8600::memalloc(nx*2);
    c128 *X = (c128*)CDC8600::memalloc(nx*2);

    for (int i = 0; i < nx; i++) 
        {
            x[i] = {drand48(),drand48()};
            X[i].real(x[i].real());
            X[i].imag(x[i].imag());
        }

    zscal_(&n, &a, x, &incx);		// Reference implementation of DCOPY
    CDC8600::BLAS::zscal(n, a, X, incx);	// Implementation of DCOPY for the CDC8600

    bool pass = true;
    for (int i = 0; i < nx; i++)
    {
        if (X[i] != x[i])
        {
            pass = false;
        }
    }

    if(traceon)
    dump(PROC[0].trace, "zscal.tr");

    cout << "zscal [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx;
    cout << ", # of instr = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
    cout << ", # of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
#ifdef FPU_TEST
    cout << "nFPU = " << setw(2) << CDC8600::params::micro::nFPUs << " , ";
#endif
#ifdef FXU_TEST
    cout << "nFXU = " << setw(2) << CDC8600::params::micro::nFXUs << " , ";
#endif
#ifdef BRU_TEST
    cout << "nBRU = " << setw(2) << CDC8600::params::micro::nBRUs << " , ";
#endif
#ifdef LDU_TEST
    cout << "nLDU = " << setw(2) << CDC8600::params::micro::nLDUs << " , ";
#endif
#ifdef STU_TEST
    cout << "nSTU = " << setw(2) << CDC8600::params::micro::nSTUs << " , ";
#endif
#ifdef LDW_TEST
    cout << "ld_latency = " << setw(2) << LD_LTC << " , "<< "ld_throughput = " << setw(2) << LD_TRP << " , ";
#endif
#ifdef STW_TEST
    cout << "st_latency = " << setw(2) << ST_LTC << " , "<< "st_throughput = " << setw(2) << ST_TRP << " , ";
#endif
#ifdef INCX_TEST
    cout << "incx = " << setw(2) << incx << " , ";
#endif
    }

int main(int argc, char **argv)
{
    if (1 == argc){
        for (int i = 0; i < N; i++)
        {
            i64 n = rand() % 256;
            c128 a = {drand48(),drand48()};
            i64 incx = (rand() % 16) - 8;
            test_zscal(i, false, n, a, incx);
        }
    }
    else if (5 == argc){
        i64 n = atoi(argv[1]); 
        c128 a = {atoi(argv[2]),atoi(argv[3])};
        i64 incx = atoi(argv[4]);
        test_zscal(0, true, n, a, incx);
    }
    else
    {
        cerr << "Usage : " << argv[0] << " [n a incx]" << endl;
        return -1;
    }
    return 0;
}
