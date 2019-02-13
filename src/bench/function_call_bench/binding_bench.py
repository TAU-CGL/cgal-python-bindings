import square
import square_swig
import square_boost
import time
import math

small = "many small tasks: "

times = 2**26
j = 1

print(small + "swig")
t1 = time.process_time()
for i in range(j, times):
    square_swig.square(i)
t2 = time.process_time()
print(t2-t1)

print(small + "boost python")
t1 = time.process_time()
for i in range(j, times):
    square_boost.square(i)
t2 = time.process_time()
print(t2-t1)

print(small + "pybind11")
t1 = time.process_time()
for i in range(j, times):
    square.square(i)
t2 = time.process_time()
print(t2-t1)

print(small + "native python")
t1 = time.process_time()
for i in range(j, times):
    math.sqrt(i)
t2 = time.process_time()
print(t2-t1)


big = "one big task: "

print(big + "swig")
t1 = time.process_time()
square_swig.squareTimes(j, times)
t2 = time.process_time()
print(t2-t1)

print(big + "boost python")
t1 = time.process_time()
square_boost.squareTimes(j, times)
t2 = time.process_time()
print(t2-t1)

print(big + "pybind11")
t1 = time.process_time()
square.squareTimes(j, times)
t2 = time.process_time()
print(t2-t1)

print(big + "native python")
t1 = time.process_time()
res = 0
for i in range(0, times):
    res += j
    res = math.sqrt(res)
t2 = time.process_time()
print(t2-t1)




