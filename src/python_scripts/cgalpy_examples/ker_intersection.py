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
Point_2 = Ker.Point_2
Segment_2 = Ker.Segment_2
s1 = Segment_2(Point_2(0, 1), Point_2(1, 0))
s2 = Segment_2(Point_2(0, 0), Point_2(1, 1))
b = Ker.do_intersect(s1, s2)
print(b)
p = Ker.intersection(s1, s2)
print(type(p))
print(p)
