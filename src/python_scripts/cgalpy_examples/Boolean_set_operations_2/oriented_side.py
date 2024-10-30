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

def print_oriented_side(obj1, obj2):
  os = Bso2.oriented_side(obj1, obj2)
  if os == Ker.Result.ON_POSITIVE_SIDE:
    print("({}) inside ({})".format(obj1, obj2))
  elif os == Ker.Result.ON_NEGATIVE_SIDE:
    print("({}) outside ({})".format(obj1, obj2))
  else:
    print("({}) on boundary ({})".format(obj1, obj2))

# Define a square polygon P and its complement Q.
P = Polygon([Point(-1, -1), Point(1, -1), Point(1, 1), Point(-1, 1)])
Q = Bso2.complement(P)

# Define the query points p1 located in P and p2 located on the boundary of P.
p1 = Point(0, 0)
p2 = Point(1, 1)

print_oriented_side(p1, P)
print_oriented_side(p2, P)
print_oriented_side(p1, Q)
print_oriented_side(p2, Q)
print_oriented_side(P, Q)
