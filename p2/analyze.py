#Khayyam Saleem and Faris Ibrahim
#ksaleem@stevens.edu and fibrahim@stevens.edu

import os
from random import randint

algorithm = ["FIFO", "LRU", "CLOCK"] #Algorithm
sign = ["+", "-"] #Prepaging
page_size = [1, 2, 4, 8, 16]

for alg in algorithm:
    for s in sign:
        for size in page_size:
            run_str = "./VMsimulator plist.txt ptrace.txt %s %s %s" % (size, alg, s)
            print(run_str)
            os.system(run_str);