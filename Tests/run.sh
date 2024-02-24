echo "----------------------FPU TEST----------------------"
make -s NFPU=0 Program=FPU_TEST && ./daxpy
make -s NFPU=1 Program=FPU_TEST && ./daxpy
make -s NFPU=2 Program=FPU_TEST && ./daxpy
make -s NFPU=4 Program=FPU_TEST && ./daxpy
make -s NFPU=8 Program=FPU_TEST && ./daxpy
echo "----------------------FXU TEST----------------------"
make -s NFXU=0 Program=FXU_TEST && ./daxpy
make -s NFXU=1 Program=FXU_TEST && ./daxpy
make -s NFXU=2 Program=FXU_TEST && ./daxpy
make -s NFXU=4 Program=FXU_TEST && ./daxpy
make -s NFXU=8 Program=FXU_TEST && ./daxpy
echo "----------------------BRU TEST----------------------"
make -s NBRU=0 Program=BRU_TEST && ./daxpy
make -s NBRU=1 Program=BRU_TEST && ./daxpy
make -s NBRU=2 Program=BRU_TEST && ./daxpy
make -s NBRU=4 Program=BRU_TEST && ./daxpy
make -s NBRU=8 Program=BRU_TEST && ./daxpy
echo "----------------------LDU TEST----------------------"
make -s NLDU=0 Program=LDU_TEST && ./daxpy
make -s NLDU=1 Program=LDU_TEST && ./daxpy
make -s NLDU=2 Program=LDU_TEST && ./daxpy
make -s NLDU=4 Program=LDU_TEST && ./daxpy
make -s NLDU=8 Program=LDU_TEST && ./daxpy
echo "----------------------STU TEST----------------------"
make -s NSTU=0 Program=STU_TEST && ./daxpy
make -s NSTU=1 Program=STU_TEST && ./daxpy
make -s NSTU=2 Program=STU_TEST && ./daxpy
make -s NSTU=4 Program=STU_TEST && ./daxpy
make -s NSTU=8 Program=STU_TEST && ./daxpy
echo "------------------LOAD LATENCY TEST------------------"
make -s RDLA=30 Program=RDLA_TEST && ./daxpy
make -s RDLA=20 Program=RDLA_TEST && ./daxpy
make -s RDLA=10 Program=RDLA_TEST && ./daxpy
make -s RDLA=1 Program=RDLA_TEST && ./daxpy
make -s RDLA=0 Program=RDLA_TEST && ./daxpy
echo "----------------LOAD THROUGHPUT TEST----------------"
make -s RDTP=10 Program=RDTP_TEST && ./daxpy
make -s RDTP=8 Program=RDTP_TEST && ./daxpy
make -s RDTP=4 Program=RDTP_TEST && ./daxpy
make -s RDTP=2 Program=RDTP_TEST && ./daxpy
make -s RDTP=1 Program=RDTP_TEST && ./daxpy
echo "-----------------STORE LATENCY TEST-----------------"
make -s STLA=30 Program=STLA_TEST && ./daxpy
make -s STLA=20 Program=STLA_TEST && ./daxpy
make -s STLA=10 Program=STLA_TEST && ./daxpy
make -s STLA=1 Program=STLA_TEST && ./daxpy
make -s STLA=0 Program=STLA_TEST && ./daxpy
echo "---------------STORE THROUGHPUT TEST---------------"
make -s STTP=10 Program=STTP_TEST && ./daxpy
make -s STTP=8 Program=STTP_TEST && ./daxpy
make -s STTP=4 Program=STTP_TEST && ./daxpy
make -s STTP=2 Program=STTP_TEST && ./daxpy
make -s STTP=1 Program=STTP_TEST && ./daxpy

