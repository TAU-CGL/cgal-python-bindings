#!/usr/bin/python3
# export PYTHONPATH=...
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

# Construct an arrangement of five linear objects.
arr1 = Arrangement()
curves = []
curves.append(Xcv(Line(Point(0, 0), Point(2, 1))))
curves.append(Xcv(Line(Point(0, 0), Point(2, -1))))
curves.append(Xcv(Line(Point(-1, 0), Point(-1, 1))))
curves.append(Xcv(Ray(Point(2, 3), Point(2, 4))))
curves.append(Xcv(Segment(Point(0, 1), Point(0, 2))))

Aos2.insert(arr1, curves)

# Print out the size of the resulting arrangement.
print('Writing an arrangement of size:')
print('  V = {} (plus {} at infinity),  E = {},  F = {} ({} unbounded)'.
      format(arr1.number_of_vertices(),
             arr1.number_of_vertices_at_infinity(),
             arr1.number_of_edges(),
             arr1.number_of_faces(),
             arr1.number_of_unbounded_faces()))

# Write the arrangement to a file.
out_file = open("arr_ex_io_unbounded.dat", 'w')
out_file.write(str(arr1))
out_file.close()

# Read the arrangement from the file.
in_file = open("arr_ex_io_unbounded.dat", 'r')
arr2 = Arrangement(in_file.read())
in_file.close()

print('Reading an arrangement of size:')
print('  V = {} (plus {} at infinity),  E = {},  F = {} ({} unbounded)'.
      format(arr2.number_of_vertices(),
             arr2.number_of_vertices_at_infinity(),
             arr2.number_of_edges(),
             arr2.number_of_faces(),
             arr2.number_of_unbounded_faces()))
