#!/bin/ksh

if [ $# -gt 0 ]; then
    set -A BLAS1 "$@"
else
    set -A BLAS1 "dtrmv_utu"
fi

make clean && clear
make run
for i in ${BLAS1[@]}; 
do 
    make $i
done

for i in ${BLAS1[@]}; 
do 
    time ./$i 1  1  1 
    time ./$i 5  5  1
    time ./$i 20 20 1
    time ./$i 50 50 1
    time ./$i 100 100 1
    time ./$i 1000 1000 1
    time ./$i 1500 1500 1
done
