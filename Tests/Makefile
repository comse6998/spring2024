BLAS_1		= dcopy zcopy drot zdrot zaxpy dscal zdotu zswap ddot zdotc idamax dasum zscal daxpy dswap
BLAS_2		= dtrans dtrmv_unn dgemv_ta dger dtrmv_ltu dtrmv_lnu dgemv_td
BLAS		= ${BLAS_1} ${BLAS_2}
TESTS 		= ${BLAS}
CCC			= g++
BLAS_DEPS 	= ../Src/blas
SRC_DEPS	= ../Src/CDC8600.cc ../Src/ISA.cc
INC_DEPS	= ../Include/CDC8600.hh
DEPS		= ${INC_DEPS} ${SRC_DEPS}
BLAS_REF	= ../Blas
ifneq ($(wildcard /opt/homebrew/Cellar/gcc/13.2.0/lib/gcc/current/.),)
FLIBS = -L/opt/homebrew/Cellar/gcc/13.2.0/lib/gcc/current/
CCFLAGS		= -I/opt/homebrew/Cellar/libomp/17.0.6/include -g -O0 -I../Include
OMPFLAGS	= -Xpreprocessor -fopenmp -L/opt/homebrew/Cellar/libomp/17.0.6/lib -lomp
else
FLIBS =
OMPFLAGS	= -fopenmp
CCFLAGS		= -Wall -g -O0 -I../Include
endif

all:	${TESTS}

blasref:
	bash -c "pushd ${BLAS_REF} && make && popd > /dev/null"

# Rule for building BLAS test cases
%::	%.cc ../Src/blas/%.cc ../Include/blas/%.hh blasref
	${CCC} ${CCFLAGS} ${SRC_DEPS} $< ../Src/blas/$< ../Blas/*.o ${FLIBS} -lgfortran -o $@

dger: dger.cc ../Src/blas/dger.cc ../Src/blas/daxpy.cc ../Include/blas/daxpy.hh ../Include/blas/dger.hh blasref
	${CCC} ${CCFLAGS} ${OMPFLAGS} ${SRC_DEPS} dger.cc ../Src/blas/dger.cc ../Src/blas/daxpy.cc ../Blas/*.o ${FLIBS} -lgfortran -o $@


dtrans:	dtrans.cc ../Src/blas/dtrans.cc ../Src/blas/dcopy.cc ../Include/blas/dcopy.hh ../Include/blas/dtrans.hh blasref
	${CCC} ${CCFLAGS} ${OMPFLAGS} ${SRC_DEPS} dtrans.cc ../Src/blas/dtrans.cc ../Src/blas/dcopy.cc ../Blas/*.o ${FLIBS} -lgfortran -o $@

dtrmv_unn:	dtrmv_unn.cc ../Src/blas/dtrmv_unn.cc ../Src/blas/ddot.cc ../Include/blas/ddot.hh ../Include/blas/dtrmv_unn.hh blasref
	${CCC} ${CCFLAGS} ${OMPFLAGS} ${SRC_DEPS} dtrmv_unn.cc ../Src/blas/dtrmv_unn.cc ../Src/blas/ddot.cc ../Blas/*.o ${FLIBS} -lgfortran -o $@

dtrmv_lnu:	dtrmv_lnu.cc ../Src/blas/dtrmv_lnu.cc ../Src/blas/ddot.cc ../Include/blas/ddot.hh ../Include/blas/dtrmv_lnu.hh blasref
	${CCC} ${CCFLAGS} ${OMPFLAGS} ${SRC_DEPS} dtrmv_lnu.cc ../Src/blas/dtrmv_lnu.cc ../Src/blas/ddot.cc ../Blas/*.o ${FLIBS} -lgfortran -o $@
	
dgemv_ta:	dgemv_ta.cc ../Src/blas/dgemv_ta.cc ../Src/blas/daxpy.cc ../Src/blas/dscal.cc ../Include/blas/daxpy.hh ../Include/blas/dscal.hh ../Include/blas/dgemv_ta.hh blasref
	${CCC} ${CCFLAGS} ${OMPFLAGS} ${SRC_DEPS} dgemv_ta.cc ../Src/blas/dgemv_ta.cc ../Src/blas/daxpy.cc ../Src/blas/dscal.cc ../Blas/*.o ${FLIBS} -lgfortran -o $@

dtrmv_ltu:	dtrmv_ltu.cc ../Src/blas/dtrmv_ltu.cc ../Src/blas/ddot.cc ../Include/blas/ddot.hh ../Include/blas/dtrmv_ltu.hh blasref
	${CCC} ${CCFLAGS} ${OMPFLAGS} ${SRC_DEPS} dtrmv_ltu.cc ../Src/blas/dtrmv_ltu.cc ../Src/blas/ddot.cc ../Blas/*.o ${FLIBS} -lgfortran -o $@

dgemv_td:	dgemv_td.cc ../Src/blas/dgemv_td.cc ../Src/blas/ddot.cc ../Src/blas/dscal.cc ../Include/blas/daxpy.hh ../Include/blas/dscal.hh ../Include/blas/dgemv_td.hh blasref
	${CCC} ${CCFLAGS} ${OMPFLAGS} ${SRC_DEPS} dgemv_td.cc ../Src/blas/dgemv_td.cc ../Src/blas/ddot.cc ../Src/blas/dscal.cc ../Blas/*.o ${FLIBS} -lgfortran -o $@

clean:
	/bin/rm -rf ${TESTS}
	bash -c "pushd ${BLAS_REF} && make clean && popd > /dev/null"
