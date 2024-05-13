#include <blas/dtrmv_unu.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" i32 dtrmv_(char *, char *, char *, i32 *, f64 *, i32 *, f64 *, i32 *);

const int N = 20;
const double EPSILON = 1e-9;

char UPLO = 'U';
char TRANS = 'N';
char DIAG = 'U';

FILE *fp;

void test_dtrmv_unu(int count, bool traceon, i32 n, i32 incx)
{
    reset();
    tracing = false;
    i32 lda = n + rand() % 64;
    f64 *A = (f64*)CDC8600::memalloc(n*lda);
    f64 *tmp1 = (f64*)CDC8600::memalloc(2*n*abs(incx));
    f64 *X = tmp1 + n*abs(incx);
    f64 *tmp = new f64[2*n*abs(incx)];
    f64 *Y = tmp + n*abs(incx);

    for (int i = 0; i < n*lda; i++) { A[i] = f64(drand48()); }
    for (int i = -n*abs(incx); i < n*abs(incx); i++) { X[i] = Y[i] = f64(drand48()); }

    dtrmv_(&UPLO, &TRANS, &DIAG, &n, A, &lda, Y, &incx);
    CDC8600::BLAS::dtrmv_unu(n, A, lda, X, incx);  

    bool pass = true;

    for (int i = -n*abs(incx); i < n*abs(incx); i+=abs(incx))
    {   
        if (abs(X[i] - Y[i]) > (min(abs(X[i]), abs(Y[i])) + EPSILON) * EPSILON)
        {
            pass = false;
        }
    }

    Y = NULL;
    delete [] tmp;
    tmp = NULL;

if(traceon){
    dump(PROC[0].trace, "dtrmv_unu.tr.0");
    dump(PROC[1].trace, "dtrmv_unu.tr.1");
    dump(PROC[2].trace, "dtrmv_unu.tr.2");
    dump(PROC[3].trace, "dtrmv_unu.tr.3");
    cout << "processor_0:"<<endl;
    dump(PROC[0].trace);
    cout << "processor_1:"<<endl;
    dump(PROC[1].trace);
    cout << "processor_2:"<<endl;
    dump(PROC[2].trace);
    cout << "processor_3:"<<endl;
    dump(PROC[3].trace);
}


    cout << "dtrmv_unu[" << setw(2) << count << "] ";
    cout << "(n = " << setw(2) << n;
    cout << ", lda = " << setw(3) << lda;
    cout << ", incx = " << setw(2) << incx;
    cout << ", # of instr  = "; for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
    cout << ", # of ops    = "; for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_count;
    cout << ", # of cycles = "; for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
// #ifndef PCNT
// #define PCNT 1
// #endif
//     if(PCNT == 1)
//     {
//         fprintf(fp, "%lu\n",PROC[0].op_maxcycle);
//     }
//     else
//     {
//         u64 max_single;
//         fscanf(fp,"%lu",&max_single);
//         f64 sp = (f64)max_single/(f64)PROC[0].op_maxcycle;  
//         cout << ", Sp = " << setw(5) << sp;
//         f64 ep = sp / PCNT*100;
//         cout << ", Ep = " << setw(5) << ep << setw(2) << "%";
//     }
    cout << ") : ";
    if (pass)
        cout << "PASS" << endl;
    else
        cout << "FAIL" << endl;
    

    //if (n < 10) dump(PROC[0].trace);

#ifdef CACHE
    cout << "Line =" << setw(3) << params::L1::linesize;
    cout << ", Sets =" << setw(3) << params::L1::nsets;
    cout << ", Ways =" << setw(3) << params::L1::nways;
    cout << ", # of cycles = ";
    for (u32 p = 0; p < params::Proc::N; p++) cout << setw(6) << PROC[p].op_maxcycle;
    cout << endl;
#endif
}

int main(int argc, char **argv)
{
    if(1 == argc){
        for (int i = 0; i < N; i++)
        {
            i32 n = rand() % 64;
            i32 incx = (rand() % 16) - 8; if (incx == 0) incx = 1;
            test_dtrmv_unu(i, false, n, incx);
        }
    }
    else if (3 == argc){
        i32 n = atoi(argv[1]);
        i32 incx = atoi(argv[2]);
        test_dtrmv_unu(0, true, n, incx);
    }
    else{
        cerr << "Usage : " << argv[0] << " [n incx]" << endl;
        return -1;
    }

#ifdef CACHE
    test_dtrmv_unu(0);
#endif

    return 0;
}

