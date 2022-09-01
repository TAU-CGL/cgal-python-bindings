#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
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
Segment = Arrangement.Geometry_traits_2.X_monotone_curve_2

p0 = Point(3, 3)
p1 = Point(1, 3)
p2 = Point(3, 5)
p3 = Point(5, 3)
p4 = Point(3, 1)
s1 = Segment(p1, p2)
s2 = Segment(p2, p3)
s3 = Segment(p3, p4)
s4 = Segment(p4, p1)
s5 = Segment(p1, p0)
s6 = Segment(p0, p3)
s7 = Segment(p4, p0)
s8 = Segment(p0, p2)

arr = Arrangement()
v0 = arr.insert_in_face_interior(p0, arr.unbounded_face())
e1 = arr.insert_in_face_interior(s1, arr.unbounded_face())
e2 = arr.insert_from_left_vertex(s2, e1)
e3 = arr.insert_from_right_vertex(s3, e2)
e4 = arr.insert_at_vertices(s4, e3, e1.twin())
e5 = arr.insert_at_vertices(s5, e1.twin(), v0)
e6 = arr.insert_at_vertices(s6, e5, e3.twin())
arr.insert_at_vertices(s7, e4.twin(), e6.twin())
arr.insert_at_vertices(s8, e5, e2.twin())

print_arrangement(arr)
