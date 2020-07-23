#!/usr/bin/python3.7
# export PYTHONPATH=...

import sys
import importlib

if len(sys.argv) < 2:
  sys.exit('Library name missing')

Tri3 = importlib.import_module(sys.argv[1]+".Tri3")
Arr2 = importlib.import_module(sys.argv[1]+".Arr2")
Ker = importlib.import_module(sys.argv[1]+".Ker")

Arrangement_2 = Arr2.Arrangement_2
Point_2 = Ker.Point_2
Curve_2 = Arr2.Curve_2

Point_3 = Ker.Point_3

p1 = Point_3(0, 0, 0)
p2 = Point_3(1, 0, 0)
p3 = Point_3(0, 1, 0)
p4 = Point_3(0, 0, 1)
p5 = Point_3(2, 2, 2)
p6 = Point_3(-1, 0, 1)

dt = Tri3.Delaunay_triangulation_3([p1, p2, p3, p4, p5, p6])
print(dt.is_valid(False, 0))
