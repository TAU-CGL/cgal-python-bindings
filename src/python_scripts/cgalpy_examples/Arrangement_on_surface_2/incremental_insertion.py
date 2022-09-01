#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
from enum import Enum
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2
Traits = Arrangement.Geometry_traits_2
Segment = Traits.X_monotone_curve_2
Naive_pl = Aos2.Arr_naive_point_location

arr = Arrangement()
pl = Naive_pl(arr)

s1 = Segment(Point(1, 0), Point(2, 4))
s2 = Segment(Point(5, 0), Point(5, 5))
s3 = Segment(Point(1, 0), Point(5, 3))
s4 = Segment(Point(0, 2), Point(6, 0))
s5 = Segment(Point(3, 0), Point(5, 5))

e = Aos2.insert_non_intersecting_curve(arr, s1, pl)
Aos2.insert_non_intersecting_curve(arr, s2, pl)
Aos2.insert(arr, s3, e.source())
# Aos2.insert(arr, s3)
Aos2.insert(arr, s4, pl)
Aos2.insert(arr, s5, pl)
print_arrangement_size(arr)

# Perform a point-location query on the resulting arrangement and print
# the boundary of the face that contains it.
q = Point(4, 1)
f = pl.locate(q)
assert(type(f) == Arrangement.Face)
print('The query point ({}) is located in: '.format(q))
print_face(f)
