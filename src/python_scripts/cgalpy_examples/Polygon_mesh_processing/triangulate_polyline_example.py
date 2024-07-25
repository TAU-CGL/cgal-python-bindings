#!/usr/bin/python
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
Point_3 = Ker.Point_3
Pmp = CGALPY.Pmp

polyline = [Point_3(0, 0, 0), Point_3(1, 0, 0), Point_3(1, 1, 0), Point_3(0, 1, 0)]
# repeating the first point (i.e. polyline.append(Point_3(0, 0, 0)) ) is optional

# a Triangle is a tuple of three integers
patch = Pmp.triangulate_hole_polyline(polyline)

for i, triangle in enumerate(patch):
  print(f"Triangle {i}: {triangle[0]} {triangle[1]} {triangle[2]}")

# note that no degenerate triangles are generated in the patch
polyline_collinear = [Point_3(0, 0, 0), Point_3(1, 0, 0), Point_3(2, 0, 0), Point_3(3, 0, 0), Point_3(4, 0, 0)]

patch_will_be_empty = Pmp.triangulate_hole_polyline(polyline_collinear)
if patch_will_be_empty:
  raise ValueError("Patch should be empty")
