#!/usr/bin/python

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

# Step (a)---construct a rectangular face.
q1 = Point(1, 3)
q2 = Point(3, 5)
q3 = Point(5, 3)
q4 = Point(3, 1)
s4 = Segment(q1, q2)
s1 = Segment(q2, q3)
s3 = Segment(q3, q4)
s2 = Segment(q4, q1)

arr = Arrangement()
e1 = arr.insert_in_face_interior(s1, arr.unbounded_face())
e2 = arr.insert_in_face_interior(s2, arr.unbounded_face())

e2 = e2.twin()   # as we wish e2 to be directed from right to left
arr.insert_at_vertices(s3, e1.target(), e2.source())
arr.insert_at_vertices(s4, e2.target(), e1.source())
print("After step (a):")
print_arrangement(arr)

# Step (b)---split e1 and e2 and connect the split points with a segment.
p1 = Point(4,4)
p2 = Point(2,2)
Segment
s1_1 = Segment(q2, p1)
s1_2 = Segment(p1, q3)
s2_1 = Segment(q4, p2)
s2_2 = Segment(p2, q1)
s = Segment(p1, p2)

e1 = arr.split_edge(e1, s1_1, s1_2)
e2 = arr.split_edge(e2, s2_1, s2_2)
e = arr.insert_at_vertices(s, e1.target(), e2.target())
print("After step (b):")
print_arrangement(arr)

# Step (c)---remove the edge e and merge e1 and e2 with their successors.
arr.remove_edge(e)
arr.merge_edge(e1, e1.next(), s1)
arr.merge_edge(e2, e2.next(), s2)
print("After step (c):")
print_arrangement(arr)
