#!/bin/ksh

if [ $# -gt 0 ]; then
    set -A BLAS1 "$@"
else
    set -A BLAS1 "daxpy dcopy ddot dscal zaxpy zdotc zdotu zdrot"
fi

make clean && clear
make run
for i in ${BLAS1[@]}; 
do 
    make $i
done

for i in ${BLAS1[@]}; 
do 
    ./$i  0  1  1 | grep $i && ./run $i 1000 && grep $i $i.pipe
    ./$i  1  1  1 | grep $i && ./run $i 1000 && grep $i $i.pipe
    ./$i 10  1  1 | grep $i && ./run $i 1000 && grep $i $i.pipe
    ./$i 10  2  3 | grep $i && ./run $i 1000 && grep $i $i.pipe
    ./$i 10 -2 -3 | grep $i && ./run $i 1000 && grep $i $i.pipe
    ./$i 10  2 -3 | grep $i && ./run $i 1000 && grep $i $i.pipe
    ./$i 10 -2  3 | grep $i && ./run $i 1000 && grep $i $i.pipe
done
