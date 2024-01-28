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
| DROTG       | ZROTG          |
| DROTMG      |                |
| DROT        | ZDROT          |
| DROTM       |                |
| DSWAP       | ZSWAP          |
| DSCAL       | ZSCAL          |
|             | ZSCAL          |
| [DCOPY](https://github.com/comse6998/spring2024/blob/main/Blas/dcopy.f)       | [ZCOPY](https://github.com/comse6998/spring2024/blob/main/Blas/zcopy.f)          |
| DAXPY       | ZAXPY          |
| DDOT        | ZDOTU          |
|             | ZDOTC          |
| DSDOT       |                |
| DNRM2       |                |
| DZNRM2      |                |
| DASUM       | DZASUM         |
| IDAMAX      | IZAMAX         |
