#!/bin/ksh

set -A DGER "dger"
set -A DGEMV "dgemv_na dgemv_nd dgemv_ta dgemv_td"
set -A DTRMV "dtrmv_unu dtrmv_utu dtrmv_lnu dtrmv_ltu dtrmv_unn dtrmv_utn dtrmv_lnn dtrmv_ltn"

if [ $# -gt 0 ]; then
    set -A BLAS2 "$@"
else
    set -A BLAS2 "$DGER $DGEMV $DTRMV"
fi

make clean && clear
make runomp
for i in ${BLAS2[@]};
do
    make $i
    export OMP_NUM_THREADS=4
    ./$i
    for j in ${DGER[@]}; 
    do
	if [ $i = $j ]; then
	    echo ""
	    ./$i 16 8 1  1 1.0 | grep $i && ./runomp $i 10000 && grep $i $i.pipe.*
	    ./$i 10 10 -2 2 0.76 | grep $i && ./runomp $i 10000 && grep $i $i.pipe.*
	fi
    done
    for j in ${DGEMV[@]};
    do
	if [ $i = $j ]; then
	    echo ""
	    ./$i 4 4 2 3 1.0 -1.0 | grep $i && ./runomp $i 10000 && grep $i $i.pipe.*
	    ./$i 10 13 2 4 3.0 -1.0 | grep $i && ./runomp $i 10000 && grep $i $i.pipe.*
	    ./$i 5 15 2 4 1.8 2.3 | grep $i && ./runomp $i 10000 && grep $i $i.pipe.*
	fi
    done
    for j in ${DTRMV[@]};
    do
	if [ $i = $j ]; then
	    echo ""
	    ./$i 15 1 | grep $i && ./runomp $i 10000 && grep $i $i.pipe.*
	    ./$i 10 2 | grep $i && ./runomp $i 10000 && grep $i $i.pipe.*
	    ./$i 13 3 | grep $i && ./runomp $i 10000 && grep $i $i.pipe.*
	fi
    done
done
