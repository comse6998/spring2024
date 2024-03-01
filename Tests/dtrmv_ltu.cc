#include <blas/dtrmv_ltu.hh>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace CDC8600;

extern "C" int32_t dtrmv_(char *, char *, char *, int32_t *, double *, int32_t *, double *, int32_t *);

const u32 N = 20;

void test_dcopy(int count)
{

    return;
}

int main()
{
    for (u32 i = 0; i < N; i++)
    {
        test_dcopy(i);
    }
    return 0;
}