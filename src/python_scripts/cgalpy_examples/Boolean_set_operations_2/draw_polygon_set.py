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

Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Polygon_with_holes = Pol2.Polygon_with_holes_2
Bso2 = CGALPY.Bso2
Polygon_set = Bso2.Polygon_set_2

# Constuct two triangular polygons and check whether they intersect.
def rectangle(l):
  return Polygon([Point(-l,-l), Point(l,-l), Point(l,l), Point(-l,l)])

# Create a large rectangle with a hole and a smaller rectangle inside the hole.
A = Polygon_with_holes(rectangle(3))
H = rectangle(2)
H.reverse_orientation()
A.add_hole(H)
B = rectangle(1)

# Add them to a polygon set and draw it.
S = Polygon_set()
S.insert(A)
S.insert(B)
Bso2.draw(S, "example")
