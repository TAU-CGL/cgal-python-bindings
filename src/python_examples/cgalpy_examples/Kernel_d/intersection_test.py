#!/usr/bin/python

import importlib
import sys

if len(sys.argv) < 2:
  lib = "CGALPY"
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Kerd = CGALPY.Kerd

Point_d = Kerd.Point_d
Segment_d = Kerd.Segment_d

p1 = Point_d(4, [0, 0, 0, 0])
p2 = Point_d(4, [1, 1, 1, 1])
p3 = Point_d(4, [0, 1, 0, 1])
p4 = Point_d(4, [1, 0, 1, 0])

s1 = Segment_d(p1, p2)
s2 = Segment_d(p3, p4)

assert Kerd.do_intersect(s1, s2)
