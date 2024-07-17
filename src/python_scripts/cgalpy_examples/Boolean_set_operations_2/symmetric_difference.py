#!/usr/bin/python3

import os
import sys
import importlib
if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]
CGALPY = importlib.import_module(lib)

sys.path.insert(1, '../')
from print_utils import *

Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Polygon_with_holes = Pol2.Polygon_with_holes_2
Bso2 = CGALPY.Bso2
Pol2 = CGALPY.Pol2

# Construct P, a bounded rectangle that contains a rectangular hole.
outP = Polygon([Point(-3, -5), Point(3, -5), Point(3, 5), Point(-3, 5)])
holesP = Polygon([Point(-1, -3), Point(-1, 3), Point(1, 3), Point(1, -3)])
P = Polygon_with_holes(outP, [holesP])
print("P =", end='')
print_polygon_with_holes(P)

# Construct Q, a bounded rectangle that contains a rectangular hole.
outQ = Polygon([Point(-5, -3), Point(5, -3), Point(5, 3), Point(-5, 3)])
holesQ = Polygon([Point(-3, -1), Point(-3, 1), Point(3, 1), Point(3, -1)])
Q = Polygon_with_holes(outQ, [holesQ])
print("Q =", end='')
print_polygon_with_holes(Q)

# Compute the symmetric difference of P and Q.
symmR = Bso2.symmetric_difference(P, Q)
print("The symmetric difference:")
for pwh in symmR:
  print("-->", end='')
  print_polygon_with_holes(pwh)
