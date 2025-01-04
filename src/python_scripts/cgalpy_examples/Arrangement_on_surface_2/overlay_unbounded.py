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
X_monotone_curve_2 = Traits.X_monotone_curve_2
Point = Traits.Point_2
Segment = Traits.Segment_2
Line = Traits.Line_2

v: Arrangement.Vertex
f: Arrangement.Face

# Construct the first arrangement, induced by two lines y = x and y = -x.
arr1 = Arrangement()	# blue
Aos2.insert(arr1, X_monotone_curve_2(Line(Point(0, 0), Point(1, 1))))
Aos2.insert(arr1, X_monotone_curve_2(Line(Point(0, 0), Point(1, -1))))

# Label the four (unbounded) faces of the arrangement as 'A' to 'D' by
# traversing the faces incident to the halfedges around the single
# arrangement vertex (0, 0).
clabel = ord('A')
v = next(arr1.vertices())
for f in v.incident_halfedges():
  f.face().set_data(chr(clabel))
  clabel = 1 + clabel

# Construct the second arrangement, containing a single square-shaped face.
arr2 = Arrangement()	# red
xcv1 = X_monotone_curve_2(Segment(Point(-3, -3), Point(3, -3)))
xcv2 = X_monotone_curve_2(Segment(Point(3, -3), Point(3, 3)))
xcv3 = X_monotone_curve_2(Segment(Point(3, 3), Point(-3, 3)))
xcv4 = X_monotone_curve_2(Segment(Point(-3, 3), Point(-3, -3)))
Aos2.insert_non_intersecting_curve(arr2, xcv1)
Aos2.insert_non_intersecting_curve(arr2, xcv2)
Aos2.insert_non_intersecting_curve(arr2, xcv3)
Aos2.insert_non_intersecting_curve(arr2, xcv4)

# Give the unbounded face the index 1, and the bounded face the index 2.
for f in arr2.faces():
  if f.is_unbounded: f.set_data(1)
  else: f.set_data(2)

# Compute the overlay of the two arrangements.
arr = Arrangement()
Ot = Aos2.Arr_overlay_traits

# You can either pass the lambda function when initializing the class object
# or, you can use one of the ten setters.
overlay_traits = Ot(lambda f1, f2, f: f.set_data(f1.data()+str(f2.data())))
# overlay_traits = Ot()
# overlay_traits.set_ff_f(lambda f1, f2, f: f.set_data(f1.data()+str(f2.data())))

Aos2.overlay(arr1, arr2, arr, overlay_traits)

# Go over the faces of the overlay arrangement and print their labels.
print("The overlay faces are:")
for f in arr.faces():
  if f.is_unbounded(): print('  {} (unbounded)'.format(f.data()))
  else: print('  {} (bounded)'.format(f.data()))
del overlay_traits
