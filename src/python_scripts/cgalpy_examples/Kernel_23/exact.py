#!/usr/bin/python

import importlib
import sys

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point = Ker.Point_2

p = Point(0, 0.3)
q = Point(1, 0.6)
r = Point(2, 0.9)
print("collinear" if Ker.collinear(p, q, r) else "not collinear")

p = Point("0 0.3")
q = Point("1 0.6")
r = Point("2 0.9")
print("collinear" if Ker.collinear(p, q, r) else "not collinear")

q = Ker.midpoint(p, r)
print("collinear" if Ker.collinear(p, q, r) else "not collinear")
