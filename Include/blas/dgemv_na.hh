#ifndef _dgemv_na_HH_
 #define _dgemv_na_HH_

 #include <CDC8600.hh>

 namespace CDC8600
 {
     namespace BLAS
     {
         void dgemv_na(u64 m, u64 n, f64 alpha, f64* A, u64 lda, f64* x, i64 incx, f64 beta, f64* y, i64 incy);
         void dgemv_na_cpp(u64 m, u64 n, f64 alpha, f64* A, u64 lda, f64* x, i64 incx, f64 beta, f64* y, i64 incy);

     } // namespace BLAS
 }; // namespace CDC8600

 #endif