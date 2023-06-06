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

class Color(Enum):
  BLUE = 0
  RED = 1
  WHITE = 2

# Construct the arrangement containing two intersecting triangles.
traits = Traits()
arr = Arrangement(traits)
Aos2.insert_non_intersecting_curve(arr, Segment(Point(4, 1), Point(7, 6)))
Aos2.insert_non_intersecting_curve(arr, Segment(Point(1, 6), Point(7, 6)))
Aos2.insert_non_intersecting_curve(arr, Segment(Point(4, 1), Point(1, 6)))
Aos2.insert(arr, Segment(Point(1, 3), Point(7, 3)))
Aos2.insert(arr, Segment(Point(1, 3), Point(4, 8)))
Aos2.insert(arr, Segment(Point(4, 8), Point(7, 3)))
Aos2.insert_point(arr, Point(4, 4.5))

# Go over all arrangement edges and set their flags.
# Recall that the value type of the edge iterator is the halfedge type.
v: Arrangement.Vertex
for v in arr.vertices():
  degree = v.degree()
  if degree == 0: v.set_data(Color.BLUE)
  elif degree == 2: v.set_data(Color.RED)
  else: v.set_data(Color.WHITE)

equal = traits.equal_2_object()
e: Arrangement.Halfedge
for e in arr.edges():
  # Check whether the halfegde has the same direction as its segment.
  flag = equal(e.source().point(), e.curve().source())
  e.set_data(flag)
  e.twin().set_data(not flag)

# Store the size of the outer boundary of every face of the arrangement.
f: Arrangement.Face
for f in arr.faces():
  boundary_size = 0
  if not f.is_unbounded():
    boundary_size = sum(1 for _ in f.outer_ccb())
  f.set_data(boundary_size)

# # Copy the arrangement and print the vertices along with their colors.
#   Ex_arrangement arr2 = arr

print("The arrangement vertices:")
for v in arr.vertices():
  print('({}) - '.format(v.point()), end='')
  color = v.data()
  if color == Color.BLUE: print("BLUE.")
  elif color == Color.RED: print("RED.")
  elif color == Color.WHITE: print("WHITE.")
  else: print('undefined color')

print("The arrangement outer-boundary sizes:", end='')
f: Arrangement.Face
for f in arr.faces(): print('', f.data(), end='')
print()
