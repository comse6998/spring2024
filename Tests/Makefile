BLAS_1		= dcopy zcopy drot zdrot zaxpy dscal zdotu zswap
BLAS		= ${BLAS_1}
TESTS 		= ${BLAS}
CCC		= g++
CCFLAGS		= -g -O0 -I../Include
BLAS_DEPS 	= ../Src/blas
SRC_DEPS	= ../Src/CDC8600.cc ../Src/ISA.cc
INC_DEPS	= ../Include/CDC8600.hh 
DEPS		= ${INC_DEPS} ${SRC_DEPS} 
BLAS_REF	= ../Blas
ifneq ($(wildcard /opt/homebrew/Cellar/gcc/13.2.0/lib/gcc/current/.),)
FLIBS = -L/opt/homebrew/Cellar/gcc/13.2.0/lib/gcc/current/
else
FLIBS =
endif

all:	${TESTS}

blasref:
	bash -c "pushd ${BLAS_REF} && make && popd > /dev/null"

# Rule for building BLAS test cases
%::	%.cc ../Src/blas/%.cc ../Include/blas/%.hh blasref
	${CCC} ${CCFLAGS} ${SRC_DEPS} $< ../Src/blas/$< ../Blas/*.o ${FLIBS} -lgfortran -o $@

clean:
	/bin/rm -rf ${TESTS}
	bash -c "pushd ${BLAS_REF} && make clean && popd > /dev/null"
