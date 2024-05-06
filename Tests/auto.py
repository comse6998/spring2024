import os
import time
#n = ["2", "6", "8", "10", "12", "14", "16", "18", "20", "22", "24", "26", "28", "30", "32", "34", "36", "38", "40", "42", "44", "46", "48", "50", "52", "54", "56", "58", "60", "62", "64", "66", "68", "70", "72", "74", "76", "78", "80", "82", "84", "86", "88", "90", "92", "94", "96", "98", "100"]
n = ["2", "6", "8", "10", "12", "14", "16", "18"]

incx = ["1", "2", "5", "-1", "-2", "-5"]
incy = ["1", "2", "5", "-1", "-2", "-5"]

print("Testing dswap - Without Trace")


os.system("make clean")
os.system("make dtrmv_utu")
os.system("make run")
os.system("export OMP_NUM_THREADS=1")
os.system("./dtrmv_utu 10 1")
os.system("./run dtrmv_utu0")
time.sleep(5)
val = input("Press 1 to continue: ")
os.system("clear")
#---------------------------
os.system("make clean")
os.system("make dtrmv_utu")
os.system("make run")
os.system("export OMP_NUM_THREADS=2")
os.system("./dtrmv_utu 10 1")
os.system("./run dtrmv_utu0")
os.system("./run dtrmv_utu1")
time.sleep(5)
val = input("Press 1 to continue: ")
os.system("clear")
#---------------------------
os.system("make clean")
os.system("make dtrmv_utu")
os.system("make run")
os.system("export OMP_NUM_THREADS=3")
os.system("./dtrmv_utu 10 1")
os.system("./run dtrmv_utu0")
os.system("./run dtrmv_utu1")
os.system("./run dtrmv_utu2")
time.sleep(5)
val = input("Press 1 to continue: ")
os.system("clear")
#---------------------------
os.system("make clean")
os.system("make dtrmv_utu")
os.system("make run")
os.system("export OMP_NUM_THREADS=4")
os.system("./dtrmv_utu 10 1")
os.system("./run dtrmv_utu0")
os.system("./run dtrmv_utu1")
os.system("./run dtrmv_utu2")
os.system("./run dtrmv_utu3")
time.sleep(5)
val = input("Press 1 to continue: ")
print ("End of Test")

    

