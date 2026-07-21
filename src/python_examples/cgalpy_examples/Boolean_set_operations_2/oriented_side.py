#!/usr/bin/python3

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Polygon_with_holes = Pol2.Polygon_with_holes_2
Bso2 = CGALPY.Bso2
Result = CGALPY.Result

def nice(os):
  if os == Result.ON_ORIENTED_BOUNDARY:
    return "on boundary"
  if os == Result.POSITIVE:
    return "inside"
  return "outside"

hole = Polygon([Point(1, 1), Point(1, 2), Point(2, 2), Point(2, 1)])
out = Polygon([Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3)])

pwh = Polygon_with_holes(out, [hole])
print(pwh)

for point in [
  Point(0, 0),
  Point(0.5, 0.5),
  Point(1, 1),
  Point(2.5, 2.5),
  Point(3, 3),
  Point(3.5, 3.5),
]:
  os = Bso2.oriented_side(point, pwh)
  print("(0,0) is : {}".format(nice(os)))
