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

    if(count == 19) {
        n = rand() % 10;
        incx = (rand() % 8) + 1;
    }

    #ifdef INCX
        incx = INCX;
    #endif

    c128 a{drand48(),drand48()};

    uint32_t nx = n*abs(incx); if (0 == nx) nx = 1;


    tracing = false; 
#ifdef TRACING
    if (n < 10 && incx >=1) tracing = true;
#endif

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

#ifdef FUNC_TEST
    cout << "zscal [" << setw(2) << count << "] (n = " << setw(3) << n << ", incx = " << setw(2) << incx << ", # of instr = " << setw(9) << instructions::count << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;
#endif
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
    
    cout << "maxcycle = " << setw (2) << operations::maxcycle << std::endl;


        //if (n < 50 && incx >=1) dump(trace);
    }

int main()
{
    //printf("FPUs = %d\n",CDC8600::params::micro::nFPUs);
#ifdef FUNC_TEST
    for (int i = 0; i < N; i++)
    {
        test_zscal(i);
    }
#else
    test_zscal(19);
#endif
    return 0;
}
