#!/usr/bin/python

import os
import sys
import importlib
import string
import random
from arr_print import *
from read_objects import *

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
Line = Traits.Line_2
Ker = CGALPY.Ker

# Get the name of the input file from the command line, or use the default
# points.dat file if no command-line parameters are given.
# try:
#   filename = argv[1]
# except:
#   filename = 'points.dat'
if len(sys.argv) > 2: filename = sys.argv[2]
else: filename = 'points.txt'

# Open the input file.
# Read the points from the file, and construct their dual lines.
# The input file format should be (all coordinate values are integers):
# <n>                               # number of point.
# <x_1> <y_1>                       # point #1.
# <x_2> <y_2>                       # point #2.
#   :      :       :      :
# <x_n> <y_n>                       # point #n.
points = read_objects(Point, filename)
dual_lines = []
for p in points:
  dual_lines.append(Xcv(Line(p.x(), -1, -p.y())))

# Construct the dual arrangement by aggregately inserting the lines.
arr = Arrangement()
Aos2.insert(arr, dual_lines)
print('The dual arrangement size:')
print('V = {} (+ {}  at infinity),  E = {},  F = {} ( unbounded)'.
      format(arr.number_of_vertices(),
             arr.number_of_vertices_at_infinity(),
             arr.number_of_edges(),
             arr.number_of_faces(),
             arr.number_of_unbounded_faces()))

# Look for a vertex whose degree is greater than 4.
found_collinear = False
v: Arrangement.Vertex
for v in arr.vertices():
  if v.degree() > 4:
    found_collinear = True
    break
if found_collinear:
  print("Found at least three collinear points in the input set.")
else:
  print("No three collinear points are found in the input set.")

# Pick two points from the input set, compute their midpoint and insert
# its dual line into the arrangement.
ker = Ker.Kernel()
n = len(points)
k1 = random.randint(0, n-1)
k2 = (k1 + 1) % n
p_mid = ker.construct_midpoint_2_object()(points[k1], points[k2])
dual_p_mid = Xcv(Line(p_mid.x(), -1, -p_mid.y()))
Aos2.insert(arr, dual_p_mid)

# Make sure that we now have three collinear points.
found_collinear = False
for v in arr.vertices():
  if v.degree() > 4:
    found_collinear = True
    break
assert(found_collinear)
