#!/usr/bin/python

import importlib
import sys

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker

segment = Ker.Segment_2(Ker.Point_2(0, 0), Ker.Point_2(2, 2))
line = Ker.Line_2(1, -1, 0)

result = Ker.intersection(segment, line)
if result:
    print(result)
