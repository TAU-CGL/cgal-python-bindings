import sp_pybind11
import swig_sp
import boost_sp
import time


times = 2**22
j = 0

print("many references to many objects - pybind11")
t1 = time.process_time()
for i in range(j, times):
    a = sp_pybind11.MyClass()
    a.set(i)
t2 = time.process_time()
print(t2-t1)


lst = [0 for i in range(times)]
print("many references to a single object - pybind11")
t1 = time.process_time()
a = sp_pybind11.MyClass()
for i in range(j, times):
    lst[i] = sp_pybind11.getAnotherReference(a)
    lst[i].set(i)
t2 = time.process_time()
print(t2-t1)


print("many references to many objects - boost")
t1 = time.process_time()
for i in range(j, times):
    a = boost_sp.MyClass()
    a.set(i)
t2 = time.process_time()
print(t2-t1)


lst = [0 for i in range(times)]
print("many references to a single object - boost")
t1 = time.process_time()
a = boost_sp.MyClass()
for i in range(j, times):
    lst[i] = boost_sp.getAnotherReference(a)
    lst[i].set(i)
t2 = time.process_time()
print(t2-t1)


print("many references to many objects - swig")
t1 = time.process_time()
for i in range(j, times):
    a = swig_sp.MyClass()
    a.set(i)
t2 = time.process_time()
print(t2-t1)


lst = [0 for i in range(times)]
print("many references to a single object - swig")
t1 = time.process_time()
a = swig_sp.MyClass()
for i in range(j, times):
    lst[i] = swig_sp.getAnotherReference(a)
    lst[i].set(i)
t2 = time.process_time()
print(t2-t1)