#!/usr/bin/python3
# export PYTHONPATH=...

import os
import sys
import importlib
from construct_segment_arrangement import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Segment = Arrangement.Geometry_traits_2.Curve_2
Point = Arrangement.Geometry_traits_2.Point_2

# Construct the arrangement.
arr = Arrangement()
# construct_segments_arr(arr)
p0 = Point(3,2)
p1 = Point(0,3)
p2 = Point(2,5)
p3 = Point(4,5)
p4 = Point(6,3)
p5 = Point(3,0)
s1 = Segment(p1, p2)
s2 = Segment(p2, p3)
s3 = Segment(p3, p4)
s4 = Segment(p4, p5)
s5 = Segment(p5, p1)

arr.insert_in_face_interior(p0, arr.unbounded_face())
e1 = arr.insert_in_face_interior(s1, arr.unbounded_face())
e2 = arr.insert_from_left_vertex(s2, e1.target())
e3 = arr.insert_from_left_vertex(s3, e2.target())
e4 = arr.insert_from_right_vertex(s4, e3.target())
arr.insert_at_vertices(s5, e4.target(), e1.source())

# Aos2.insert(arr, Segment(Point(3,0), Point(0,3)))
Aos2.draw(arr)

# Perform a batched point-location query.
points = [Point(1, 4), Point(4, 3), Point(6, 3), Point(3, 2), Point(5, 2), Point(1, 0)]
res = Aos2.locate(arr, points)

# Print the results.
for element in res:
  print(element)
