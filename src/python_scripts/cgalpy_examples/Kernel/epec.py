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
Kernel = Ker.Kernel
kernel = Kernel()
cc_in_between_2 = kernel.counterclockwise_in_between_2_object()
p0 = Point_2(0,0)
p1 = Point_2(1,0)
p2 = Point_2(1,1)
p3 = Point_2(0,1)
v1 = p1 - p0
v2 = p2 - p0
v3 = p3 - p0
Direction_2 = Ker.Direction_2
d1 = Direction_2(v1)
d2 = Direction_2(v2)
d3 = Direction_2(v3)
rc = cc_in_between_2(d2, d1, d3)
print(rc)
