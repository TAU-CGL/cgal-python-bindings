#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
FT = Ker.Gmpq
FT = Ker.FT
Gmpq = Ker.Gmpq
Point = Ker.Point_2

x = FT(2,2)
y = FT(6,2)
print(Point.__doc__)
p1 = Point(1, 3)
p2 = Point(2, 6)
p3 = Point(x, y)
print("seed: " + str(hex(hash(p1))))
print("seed: " + str(hex(hash(p2))))
print("seed: " + str(hex(hash(p3))))
