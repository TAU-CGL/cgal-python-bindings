#!/usr/bin/python3

import os
import sys
import importlib
from print_utils import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Bso2 = CGALPY.Bso2

# Construct two triangular polygons and check whether they intersect.
P = Polygon([Point(-1, 1), Point(0, -1), Point(1, 1)])
print("P = ", end='')
print_polygon(P)

Q = Polygon([Point(-1, -1), Point(1, -1), Point(0, 1)])
print("Q = ", end='')
print_polygon(Q)

if Bso2.do_intersect(P, Q):
  print("The two polygons intersect in their interior.")
else:
  print("The two polygons do not intersect.")
