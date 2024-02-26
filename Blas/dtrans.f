      SUBROUTINE DTRANS(M,N,A,LDA,B,LDB)
*     .. Scalar Arguments ..
      INTEGER M,N,LDA,LDB
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION A(LDA,*),B(LDB,*)
*     ..
*
*  Purpose
*  =======
*
*  DTRAN transposes the N x M section of B(LDB,*) into
*  the M x N section of A(LDA,*)
*
*  Arguments
*  ==========
*
*
*  M      - INTEGER.
*           On entry, M specifies the number of rows of the matrix A.
*           This is the same as the number of columns of the matrix B.
*           M must be at least zero.
*           Unchanged on exit.
*
*  N      - INTEGER.
*           On entry, N specifies the number of columns of the matrix A.
*           This is the same as the number of rows of the matrix B.
*           N must be at least zero.
*           Unchanged on exit.
*
*  A      - DOUBLE PRECISION array of DIMENSION ( LDA, n ).
*           The leading m by n part of the array A is overwritten
*           by the contents of the leading n by m part of the array B.
*
*  LDA    - INTEGER.
*           On entry, LDA specifies the first dimension of A as declared
*           in the calling (sub) program. LDA must be at least
*           max( 1, m ).
*           Unchanged on exit.
*
*  B      - DOUBLE PRECISION array of DIMENSION ( LDB, m ).
*           The leading n by m part of the array B contains the source
*           data.
*           Unchanged on exit.
*
*  LDB    - INTEGER.
*           On entry, LDB specifies the first dimension of B as declared
*           in the calling (sub) program. LDB must be at least
*           max( 1, m ).
*           Unchanged on exit.
*
*  =====================================================================
*
      INTEGER I, J

      DO I = 1, M
        DO J = 1, N
          A(I,J) = B(J,I)
        END DO
      END DO
      RETURN
*
*     End of DTRANS .
*
      END
