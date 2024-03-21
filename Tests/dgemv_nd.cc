#include <blas/dgemv_nd.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <omp.h>

using namespace CDC8600;

extern "C" i32 dgemv_(char *, i32 *, i32 *, f64 *, f64 *, i32 *, f64 *, i32 *, f64 *, f64 *, i32 *);

const int N = 20;
const double epsilon = std::pow(10, -6);
const int NUM_THREADS = 4;
bool PRETTY_PRINT_CORES = true;

// Global variables to store total instructions and cycles
uint64_t totalInstructions = 0;
uint64_t totalCycles = 0;
uint64_t maxInstructions = 0;
uint64_t maxCycles = 0;

void printPerformanceMetrics(int coreCount, bool pass) {
    std::cout << std::left << std::setfill(' '); // Align text to the left and set fill character
    // Print headers
    std::cout << std::setw(10) << "Core" << std::setw(20) << "# of Instructions" << std::setw(20) << "# of Cycles" << std::setw(10) << "Result" << std::endl;
    std::cout << std::setfill('-') << std::setw(60) << "" << std::setfill(' ') << std::endl; // Print a dividing line

    for (int i = 0; i < coreCount; ++i) {
        std::cout << std::setw(10) << i; // Print core number
        std::cout << std::setw(20) << PROC[i].instr_count; // Print number of instructions
        std::cout << std::setw(20) << PROC[i].op_maxcycle; // Print number of cycles
        std::cout << std::setw(10) << (pass ? "PASS" : "FAIL") << std::endl; // Print pass/fail
    }
    printf("\n");
}

void test_dgemv_nd(int count) {
    reset();

    i32 m = rand() % 256;
    i32 n = rand() % 256;
    i32 lda = m + rand() % 256;

    i32 incx = (rand() % 16) - 8; if (incx == 0) incx = 1;
    u32 nx = n*abs(incx);
    if (0 == nx) nx = 1;

    i32 incy = (rand() % 16) - 8; if (incy == 0) incy = 1;
    u32 ny = m*abs(incy);
    if (0 == ny) ny = 1;

    f64 alpha = drand48();
    f64 beta = drand48();

    f64 *a = (f64*)CDC8600::memalloc(lda*n);
    f64 *x = (f64*)CDC8600::memalloc(nx);
    f64 *y = (f64*)CDC8600::memalloc(ny);
    f64 *y_ = (f64*)CDC8600::memalloc(ny);

    for (i32 i = 0; i < lda*n; i++) { a[i] = drand48(); }
    for (u32 i = 0; i < nx; i++) {    x[i] = drand48(); }
    for (u32 i = 0; i < ny; i++) {    y_[i] = y[i] = drand48(); }

    dgemv_((char *) "n", &m, &n, &alpha, a, &lda, x, &incx, &beta, y, &incy);

    CDC8600::BLAS::dgemv_nd(m, n, alpha, a, lda, x, incx, beta, y_, incy);

    bool pass = true;
    for (u32 i = 0; i < ny; i++)
    {
        if (abs(y[i] - y_[i]) > (min(abs(y[i]), abs(y_[i])) + epsilon) * epsilon)
	{
            pass = false;
        }

    }

    if (PRETTY_PRINT_CORES){
        printf("RUN %i \n", count);
        printPerformanceMetrics(params::Proc::N, pass);
    } else {
        cout << "dgemv_nd [" << setw(2) << count << "] ";
        cout << "(m = " << setw(3) << m;
        cout << ", n = " << setw(3) << n;
        //cout << ", alpha = " << setw(2) << alpha;
        //cout << ", LDA = " << setw(2) << LDA;
        //cout << ", incx = " << setw(2) << incx;
        //cout << ", beta = " << setw(2) << beta;
        //cout << ", incy = " << setw(2) << incy;
        cout << ", # of instr = ";
        for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].instr_count;
        cout << ", # of cycles = ";
        for (u32 p = 0; p < params::Proc::N; p++) cout << setw(9) << PROC[p].op_maxcycle;
        cout << ") : ";

        if (pass)
            cout << "PASS" << std::endl;
        else
            cout << "FAIL" << std::endl;
    }

    // Update global total instructions and cycles
    for (u32 p = 0; p < params::Proc::N; p++) {
        totalInstructions += PROC[p].instr_count;
        totalCycles += PROC[p].op_maxcycle;

        // Update max instructions and cycles
        if (PROC[p].instr_count > maxInstructions) {
            maxInstructions = PROC[p].instr_count;
        }
        if (PROC[p].op_maxcycle > maxCycles) {
            maxCycles = PROC[p].op_maxcycle;
        }
    }
}

int main() {
    for (int i = 0; i < N; i++) {
        test_dgemv_nd(i);
    }

    // Calculate averages
    int usedCores = (PRETTY_PRINT_CORES) ? params::Proc::N : NUM_THREADS;
    double avgInstructions = static_cast<double>(totalInstructions) / usedCores / N;
    double avgCycles = static_cast<double>(totalCycles) / usedCores / N;

    // Print averages
    std::cout << "Average Instructions per Core: " << avgInstructions << std::endl;
    std::cout << "Average Cycles per Core: " << avgCycles << std::endl;

    // Print max instructions and cycles
    std::cout << "Max Instructions: " << maxInstructions << std::endl;
    std::cout << "Max Cycles: " << maxCycles << std::endl;

    return 0;
}
