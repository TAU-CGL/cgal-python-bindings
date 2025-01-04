#!/usr/bin/python

import os
import sys
import importlib
import string
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Aos2.Arr_linear_traits_2
Xcv = Traits.X_monotone_curve_2
Point = Traits.Point_2
Segment = Traits.Segment_2
Line = Traits.Line_2
Ray = Traits.Ray_2

arr = Arrangement()

# Insert a line in the (currently single) unbounded face of the arrangement
# then, insert a point that lies on the line splitting it into two.
c1 = Xcv(Line(Point(-1, 0), Point(1, 0)))
arr.insert_in_face_interior(c1, arr.unbounded_face())
v = Aos2.insert_point(arr, Point(0,0))
assert(not v.is_at_open_boundary())

# Add two more rays using the specialized insertion functions.
arr.insert_from_right_vertex(Xcv(Ray(Point(0, 0), Point(-1, 1))), v) # c2
arr.insert_from_left_vertex(Xcv(Ray(Point(0, 0), Point(1, 1))), v)   # c3

# Insert three more interior-disjoint rays, c4, c5, and c6.
Aos2.insert_non_intersecting_curve(arr, Xcv(Ray(Point(0, -1), Point(-2, -2))))
Aos2.insert_non_intersecting_curve(arr, Xcv(Ray(Point(0, -1), Point(2, -2))))
Aos2.insert_non_intersecting_curve(arr, Xcv(Ray(Point(0, 0), Point(0, 1))))

print_unbounded_arrangement_size(arr)

# Print the outer CCBs of the unbounded faces.
k = 0
f: Arrangement.Face
for f in arr.unbounded_faces():
  k = k + 1
  print('Face no. {}({},{}): '.format(k, f.is_unbounded(), f.number_of_holes()), end='')
  ccb = f.outer_ccb()
  e = next(ccb)
  if not e.source().is_at_open_boundary():
    print('({})'.format(e.source().point()), end='')
  for e in ccb:
    if not e.is_fictitious(): print(' [{}]'.format(e.curve()), end='')
    else: print (" [ ... ] ", end='')

    if not e.target().is_at_open_boundary():
      print('({})'.format(e.target().point()), end='')
  print()
