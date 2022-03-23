#!/usr/bin/python3
# export PYTHONPATH=...

import os
import sys
import importlib

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY_kerEpic_pol2_tri3DelAsAsComSeq'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.ker
Tri3 = CGALPY.tri3
Point_3 = Ker.Point_3
p1 = Point_3(0, 0, 0)
p2 = Point_3(1, 0, 0)
p3 = Point_3(0, 1, 0)
p4 = Point_3(0, 0, 1)
p5 = Point_3(2, 2, 2)
p6 = Point_3(-1, 0, 1)
dt = Tri3.Triangulation_3([p1, p2, p3, p4, p5, p6])
print(dt.is_valid(False, 0))
