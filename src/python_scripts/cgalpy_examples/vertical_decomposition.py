#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement_2 = Aos2.Arrangement_2
Point_2 = Arrangement_2.Geometry_traits_2.Point_2
Curve_2 = Arrangement_2.Geometry_traits_2.Curve_2

arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(3, 3))
c2 = Curve_2(Point_2(3, 3), Point_2(6, 0))
c3 = Curve_2(Point_2(2, 0), Point_2(5, 3))
c4 = Curve_2(Point_2(5, 3), Point_2(8, 0))
Aos2.insert(arr, [c1, c2, c3, c4])
d = Aos2.decompose(arr)
for pair in d:
  v0 = pair[0]
  print(v0.point())
  for obj in pair[1]:
    if type(obj) is Arrangement_2.Vertex: print("  ", obj.point())
    elif type(obj) is Arrangement_2.Halfedge: print("  ", obj.curve())
    elif type(obj) is Arrangement_2.Face: print("  unbounded face")
    else: print("  none")
