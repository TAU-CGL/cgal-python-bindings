#!/usr/bin/python3
# export PYTHONPATH=swig:pybind11:boost:nanobind

import sp_nanobind
import sp_pybind11
import swig_sp
import sp_boost
import time

times = 2**22
j = 0

# Nanobind
print("many references to many objects - nanobind")
t1 = time.process_time()
for i in range(j, times):
    a = sp_nanobind.MyClass()
    a.set(i)
t2 = time.process_time()
print("%.3f" % float(t2-t1))

lst = [0 for i in range(times)]
print("many references to a single object - nanobind")
t1 = time.process_time()
a = sp_nanobind.MyClass()
for i in range(j, times):
    lst[i] = sp_nanobind.getAnotherReference(a)
    lst[i].set(i)
t2 = time.process_time()
print("%.3f" % float(t2-t1))

# PyBind11
print("many references to many objects - pybind11")
t1 = time.process_time()
for i in range(j, times):
    a = sp_pybind11.MyClass()
    a.set(i)
t2 = time.process_time()
print("%.3f" % float(t2-t1))

lst = [0 for i in range(times)]
print("many references to a single object - pybind11")
t1 = time.process_time()
a = sp_pybind11.MyClass()
for i in range(j, times):
    lst[i] = sp_pybind11.getAnotherReference(a)
    lst[i].set(i)
t2 = time.process_time()
print("%.3f" % float(t2-t1))

# Boost
print("many references to many objects - boost")
t1 = time.process_time()
for i in range(j, times):
    a = sp_boost.MyClass()
    a.set(i)
t2 = time.process_time()
print("%.3f" % float(t2-t1))

lst = [0 for i in range(times)]
print("many references to a single object - boost")
t1 = time.process_time()
a = sp_boost.MyClass()
for i in range(j, times):
    lst[i] = sp_boost.getAnotherReference(a)
    lst[i].set(i)
t2 = time.process_time()
print("%.3f" % float(t2-t1))

# Swig
print("many references to many objects - swig")
t1 = time.process_time()
for i in range(j, times):
    a = swig_sp.MyClass()
    a.set(i)
t2 = time.process_time()
print("%.3f" % float(t2-t1))


lst = [0 for i in range(times)]
print("many references to a single object - swig")
t1 = time.process_time()
a = swig_sp.MyClass()
for i in range(j, times):
    lst[i] = swig_sp.getAnotherReference(a)
    lst[i].set(i)
t2 = time.process_time()
print("%.3f" % float(t2-t1))
