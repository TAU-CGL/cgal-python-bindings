#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import arr_print as ap

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2
Segment = Arrangement.Geometry_traits_2.X_monotone_curve_2
s1 = Segment(Point(1, 0), Point(2, 4))
s2 = Segment(Point(5, 0), Point(5, 5))
s3 = Segment(Point(1, 0), Point(5, 3))
s4 = Segment(Point(0, 2), Point(6, 0))
s5 = Segment(Point(3, 0), Point(5, 5))
arr = Arrangement()
Aos2.insert(arr, [s1, s2, s3, s4, s5])
ap.print_arrangement_size(arr)
