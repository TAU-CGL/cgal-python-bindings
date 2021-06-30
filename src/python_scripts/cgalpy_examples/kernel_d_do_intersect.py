#!/usr/bin/python3.9
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

Kerd = CGALPY.Kerd
Point_d = Kerd.Point_d
Segment_d = Kerd.Point_d
p11 = Point_d(4, [FT(n) for n in [0, 0, 0, 0]])
p12 = Point_d(4, [FT(n) for n in [1, 1, 1, 1]])
s1 = Segment_d(p11, p12)
p21 = Point_d(4, [FT(n) for n in [1, 0, 1, 0]])
p22 = Point_d(4, [FT(n) for n in [0, 1, 0, 1]])
s2 = Segment_d(p21, p22)
print(Kerd.do_intersect(s1, s2)
