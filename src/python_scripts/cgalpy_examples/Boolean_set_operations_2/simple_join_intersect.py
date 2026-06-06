#!/usr/bin/python

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
Polygon_with_holes = Pol2.Polygon_with_holes_2
Bso2 = CGALPY.Bso2

# Construct the two input polygons.
P = Polygon([Point(0, 0), Point(5, 0), Point(3.5, 1.5), Point(2.5, 0.5),
             Point(1.5, 1.5)])
print("P = ", end='')
print_polygon(P)

Q = Polygon([Point(0, 2), Point(1.5, 0.5), Point(2.5, 1.5), Point(3.5, 0.5),
             Point(5, 2)])
print("Q = ", end='')
print_polygon(Q)

# Compute the union of P and Q.
unionR = Polygon_with_holes()
if Bso2.join(P, Q, unionR):
  print("The union: ", end='')
  print_polygon_with_holes(unionR)
else:
  print("P and Q are disjoint and their union is trivial.")

print()

# Compute the intersection of P and Q.
intR = Bso2.intersection(P, Q)

print("The intersection:")
for pwh in intR:
  print('--> ', end='')
  print_polygon_with_holes(pwh)
