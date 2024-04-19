#!/bin/ksh

set -A BLAS1 "daxpy dcopy ddot"

make clean && clear
make run
for i in ${BLAS1[@]}; 
do 
    make $i
done

for i in ${BLAS1[@]}; 
do 
    ./$i  0  1  1 | grep $i && ./run $i.tr 1000 > $1.pipe && grep $1.tr $1.pipe
    ./$i  1  1  1 | grep $i && ./run $i.tr 1000 > $1.pipe && grep $1.tr $1.pipe
    ./$i 10  1  1 | grep $i && ./run $i.tr 1000 > $1.pipe && grep $1.tr $1.pipe
    ./$i 10  2  3 | grep $i && ./run $i.tr 1000 > $1.pipe && grep $1.tr $1.pipe
    ./$i 10 -2 -3 | grep $i && ./run $i.tr 1000 > $1.pipe && grep $1.tr $1.pipe
    ./$i 10  2 -3 | grep $i && ./run $i.tr 1000 > $1.pipe && grep $1.tr $1.pipe
    ./$i 10 -2  3 | grep $i && ./run $i.tr 1000 > $1.pipe && grep $1.tr $1.pipe
done
