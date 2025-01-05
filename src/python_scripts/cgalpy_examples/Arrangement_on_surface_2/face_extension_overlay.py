#!/usr/bin/python

import os
import sys
import importlib
import string
from arr_print import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Aos2.Arr_segment_traits_2
Point = Traits.Point_2
Segment = Traits.X_monotone_curve_2

f: Arrangement.Face

# typedef CGAL::Arr_face_overlay_traits<Ex_arrangement, Ex_arrangement,
#                                       Ex_arrangement,
#                                       std::logical_and<bool> >
#                                                          Overlay_traits;

# Construct the first arrangement, containing a square-shaped face.
arr1 = Arrangement()
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(2, 2), Point(6, 2)))
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(6, 2), Point(6, 6)))
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(6, 6), Point(2, 6)))
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(2, 6), Point(2, 2)))
# Bounded and the unbounded faces
assert(arr1.number_of_faces() == 2)

# Mark just the bounded face.
for f in arr1.faces():
  f.set_data(not f.is_unbounded())

# Construct the second arrangement, containing a rhombus-shaped face.
arr2 = Arrangement()
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(4, 1), Point(7, 4)))
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(7, 4), Point(4, 7)))
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(4, 7), Point(1, 4)))
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(1, 4), Point(4, 1)))

for f in arr2.faces(): f.set_data(not f.is_unbounded())

# Compute the overlay of the two arrangements, marking only the faces that
# are intersections of two marked faces in arr1 and arr2, respectively.
arr = Arrangement()
Ot = Aos2.Arr_overlay_traits
overlay_traits = Ot(lambda f1, f2, f: f.set_data(f1.data() and f2.data()))
Aos2.overlay(arr1, arr2, arr, overlay_traits)

# Go over the faces of the resulting arrangement and print the marked ones.
print("The intersection is: ", end='')
for f in arr.faces():
  if not f.data(): continue
  ccb = f.outer_ccb()
  e = next(ccb)
  print(e.source().point(), end='')
  for e in ccb: print(' -> {}'.format(e.target().point()), end='')
  print()
del overlay_traits
