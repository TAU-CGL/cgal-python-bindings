#!/usr/bin/python3.7
# export PYTHONPATH=...

import sys
import importlib

if len(sys.argv) < 2:
    sys.exit('Library name missing')

Arr2 = importlib.import_module(sys.argv[1]+".Arr2")
Ker = importlib.import_module(sys.argv[1]+".Ker")

Arrangement_2 = Arr2.Arrangement_2
Point_2 = Ker.Point_2
Segment_2 = Ker.Segment_2
Curve_2 = Arr2.Curve_2

arr = Arrangement_2()
p0 = Point_2(0, 0)
p1 = Point_2(1, 0)
p2 = Point_2(1, 1)
p3 = Point_2(0, 1)

Arr2.insert(arr, Curve_2(Segment_2(p0, p1)))
Arr2.insert(arr, Curve_2(Segment_2(p1, p2)))
Arr2.insert(arr, Curve_2(Segment_2(p2, p3)))
Arr2.insert(arr, Curve_2(Segment_2(p3, p0)))

res = []
c = Curve_2(Segment_2(Point_2(0, 0.5), Point_2(1, 1.5)))
Arr2.zone(arr, c, res)
