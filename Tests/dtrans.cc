#include <blas/dtrans.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t dtrans_(int32_t *, int32_t *, double *, int32_t *, double *, int32_t *);

const int N = 20;

void test_dtrans(int count)
{
    reset();

    int32_t m = rand() % 256;
    int32_t n = rand() % 256;
    int32_t mn = m * n;
    
    tracing = false;

    f64 *A = (f64*)CDC8600::memalloc(mn);	// Output matrix
    f64 *B = (f64*)CDC8600::memalloc(mn);	// Input matrix
    f64 *C = new f64[mn];			// Reference matrix

    for (int i = 0; i < mn; i++) { B[i] = drand48(); }
    for (int i = 0; i < mn; i++) { A[i] = 0.0;	 }
    for (int i = 0; i < mn; i++) { C[i] = 0.0;	 }

    dtrans_(&m, &n, C, &m, B, &n);		// Reference implementation of DTRANS
    CDC8600::BLAS::dtrans(m, n, A, m, B, n);	// Implementation of DTRANS for the CDC8600

    bool pass = true;
    for (int i = 0; i < mn; i++)
    {
        if (A[i] != C[i])
        {
            pass = false;
        }
    }

    delete [] C;

    cout << "dtrans [" << setw(2) << count << "] ";
    cout << "(m = " << setw(3) << m;
    cout << ", n = " << setw(3) << n;
    cout << ", # of instr = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
    cout << ", # of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
    cout << ") : ";
    if (pass)
        cout << "PASS" << std::endl;
    else
        cout << "FAIL" << std::endl;

    if (n < 10) dump(PROC[0].trace);
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        test_dtrans(i);
    }
    return 0;
}
