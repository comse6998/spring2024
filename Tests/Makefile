NFPU = 1
NFXU = 1
NBRU = 1
NLDU = 1
NSTU = 1
RDLA = 30
RDTP = 1
STLA = 30
STTP = 1
Program = 
BLAS_1		= daxpy
BLAS		= ${BLAS_1}
TESTS 		= ${BLAS}
CCC			= g++
PARA = -D_NFPU_=$(NFPU) -D_NFXU_=$(NFXU) -D_NBRU_=$(NBRU) -D_NSTU_=$(NSTU) -D_NLDU_=$(NLDU) -D${Program} -w \
		-D_RDLA_=$(RDLA) -D_RDTP_=$(RDTP)  -D_STLA_=$(STLA) -D_STTP_=$(STTP)
CCFLAGS		= -g -O0 -I../Include ${PARA}
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
