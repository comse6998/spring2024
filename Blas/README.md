# BLAS (Basic Linear Algebra Subprograms) [https://www.netlib.org/blas/](https://www.netlib.org/blas/)

You can find the documentation [here](https://netlib.org/lapack/explore-html/dir_fa94b7b114d387a7a8beb2e3e22bf78d.html) and the source files in [github](https://github.com/xianyi/netlib_blas).

# Introduction

The BLAS (Basic Linear Algebra Subprograms) is a *de facto* standard library of functions that can be used as building blocks for numerical linear algebra algorithms. 
The above website has the full definition of the BLAS. 
We are going to work with only a small subset of the BLAS.

# For the class

We will start by focusing on LEVEL 1 BLAS routines in double and double complex precision.

| Double real | Double complex |
|-------------|----------------|
| [DROT](https://github.com/comse6998/spring2024/blob/main/Blas/drot.f)       | [ZDROT](https://github.com/comse6998/spring2024/blob/main/Blas/zdrot.f)        |
| [DSWAP](https://github.com/comse6998/spring2024/blob/main/Blas/dswap.f)     | [ZSWAP](https://github.com/comse6998/spring2024/blob/main/Blas/zswap.f)        |
| [DSCAL](https://github.com/comse6998/spring2024/blob/main/Blas/dscal.f)     | [ZSCAL](https://github.com/comse6998/spring2024/blob/main/Blas/zscal.f)        |
| [DCOPY](https://github.com/comse6998/spring2024/blob/main/Blas/dcopy.f)     | [ZCOPY](https://github.com/comse6998/spring2024/blob/main/Blas/zcopy.f)        |
| [DAXPY](https://github.com/comse6998/spring2024/blob/main/Blas/daxpy.f)     | [ZAXPY](https://github.com/comse6998/spring2024/blob/main/Blas/zaxpy.f)        |
| [DDOT](https://github.com/comse6998/spring2024/blob/main/Blas/ddot.f)       | [ZDOTU](https://github.com/comse6998/spring2024/blob/main/Blas/zdotu.f)        |
|                                                                             | [ZDOTC](https://github.com/comse6998/spring2024/blob/main/Blas/zdotc.f)        |
| [DASUM](https://github.com/comse6998/spring2024/blob/main/Blas/dasum.f)     |                                                                                |
| [IDAMAX](https://github.com/comse6998/spring2024/blob/main/Blas/idamax.f)   |                                                                                |
