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
  def __str__(self):
    if self.value == 0: return "BLUE"
    elif self.value == 1: return "RED"
    else: return "WHITE"

# Construct the arrangement containing two intersecting triangles.
traits = Traits()
arr = Arrangement(traits)

s1 = Segment(Point(4, 1), Point(7, 6))
s2 = Segment(Point(1, 6), Point(7, 6))
s3 = Segment(Point(4, 1), Point(1, 6))
s4 = Segment(Point(1, 3), Point(7, 3))
s5 = Segment(Point(1, 3), Point(4, 8))
s6 = Segment(Point(4, 8), Point(7, 3))

Aos2.insert_non_intersecting_curve(arr, s1)
Aos2.insert_non_intersecting_curve(arr, s2)
Aos2.insert_non_intersecting_curve(arr, s3)
Aos2.insert(arr, s4)
Aos2.insert(arr, s5)
Aos2.insert(arr, s6)

# Go over all arrangement vertices and set their colors.
v: Arrangement.Vertex
for v in arr.vertices():
  degree = v.degree()
  if degree == 0: v.set_data(Color.BLUE)    	# Isolated vertex
  elif degree <= 2: v.set_data(Color.RED)	# Vertex represents an endpoint
  else: v.set_data(Color.WHITE)    # Vertex represents an intersection point

# Go over all arrangement edges and set their flags.
equal = traits.equal_2_object()
e: Arrangement.Halfedge
for e in arr.edges():
  # Check if the halfedge has the same direction as its associated
  # segment. Note that its twin always has an opposite direction.
  flag = equal(e.source().point(), e.curve().source())
  e.set_data(flag)
  e.twin().set_data(not flag)

# Go over all arrangement faces and print their outer boundary and indices.
f: Arrangement.Face
for f in arr.faces():
  boundary_size = 0
  if not f.is_unbounded():
    boundary_size = sum(1 for _ in f.outer_ccb())
  f.set_data(boundary_size)

# Write the arrangement to a file.
out_file = open("arr_ex_dcel_io.dat", 'w')
out_file.write(str(arr))
out_file.close()

# Formatter formatter
# CGAL::IO::write(arr, out_file, formatter)

# Read the arrangement from the file.
in_file = open("arr_ex_dcel_io.dat", 'r')
arr2 = Arrangement(in_file.read())
in_file.close()

print("The arrangement vertices:")

# Observe that we always write (and read) auxliary data as text. It implies,
# for example, that the Python color enumerations stored at the vertices
# of the first arrangement are written to the temporary text file as text; then
# they are read back as text, and stored at the vertices of the second
# arrangement as text. Below we convert the color texts back to color
# enumerations
for v in arr2.vertices():
  # v.set_data(Color(v.data()))
  color = v.data()
  if color == "BLUE": v.set_data(Color.BLUE)
  elif color == "RED": v.set_data(Color.RED)
  elif color == "WHITE": v.set_data(Color.WHITE)
  else: raise Exception('Unrecognized color {}'.format(color))

  print('({}) - {}'.format(v.point(), v.data()))
