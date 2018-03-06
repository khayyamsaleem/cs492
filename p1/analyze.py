#Khayyam Saleem and Faris Ibrahim
#ksaleem@stevens.edu and fibrahim@stevens.edu

import os
from random import randint


##--------------------------FIRST COME FIRST SERVE-----------------------------------
num_products = 100
num_producers = [5,5,50]
num_consumers = [5,50,5]
prod_cons = zip(num_producers, num_consumers)
queue_size = [10, 50, 0]

#  output format
#  time elapsed
#  turn around min
#  turn around max
#  turn around avg
#  wait time min
#  wait time max
#  wait time avg
#  producer throughput
#  consumer throughput

seeds = [40, 29, 74]

#call for 100 products
for p,c in prod_cons:
    for q in queue_size:
        for seed in seeds:
            run_str = "./prodcons %s %s %s %s %s %s %s | tail -9 | xargs | tr ' ' ',' | sed 's/$/,/' >> fcfs_100.csv" % (p, c, 100, q, 0, 0, seed)
            print(run_str)
            os.system(run_str)

#call for 5000 products
num_producers = [50,50,1000]
num_consumers = [50,1000,50]
prod_cons = zip(num_producers, num_consumers)
queue_size = [100,1000,0]
for p,c in prod_cons:
    for q in queue_size:
        for seed in seeds:
            run_str = "./prodcons %s %s %s %s %s %s %s | tail -9 | xargs | tr ' ' ',' | sed 's/$/,/' >> fcfs_5000.csv" % (p, c, 5000, q, 0, 0, seed)
            print(run_str)
            os.system(run_str)

##-------------------------------ROUND ROBIN------------------------------

num_products = 100
num_producers = [5,5,50]
num_consumers = [5,50,5]
prod_cons = zip(num_producers, num_consumers)
queue_size = [10,50,0]
quantum = [100, 512, 900]

seeds = [29, 12, 67]
# call for 100 products
for p,c in prod_cons:
    for q in queue_size:
        for quant in quantum:
            for seed in seeds:
                run_str = "./prodcons %s %s %s %s %s %s %s | tail -9 | xargs | tr ' ', ',' | sed 's/$/,/' >> rr_100.csv" % (p, c, 100, q, 1, quant, seed)
                print(run_str)
                os.system(run_str)

# call for 5000 products
num_producers = [50,50,1000]
num_consumers = [50,1000,50]
prod_cons = zip(num_producers, num_consumers)
queue_size = [100, 1000, 0]
for p,c in prod_cons:
    for q in queue_size:
        for quant in quantum:
            for seed in seeds:
                run_str = "./prodcons %s %s %s %s %s %s %s | tail -9 | xargs | tr ' ', ',' | sed 's/$/,/' >> rr_5000.csv" % (p, c, 5000, q, 1, quant, seed)
                print(run_str)
                os.system(run_str)

