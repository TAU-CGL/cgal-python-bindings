#!/usr/bin/python

import os
import sys
import importlib
from dump_to_eps import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]
CGALPY = importlib.import_module(lib)

Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Polygon_with_holes = Pol2.Polygon_with_holes_2
Sn2 = CGALPY.Sn2

# Obtain the polygon with holes file name
try: filename = sys.argv[2]
except: filename = 'sample_1.dat'

# Obtain the offset
try: offset = float(sys.argv[3])
except: offset = 0.25

# Obtain the eps file name
try: eps_filename = sys.argv[4]
except: eps_filename = filename + ".skeleton.eps"

# Read the polygon with holes
in_file = open(filename, 'r')
pwh = Polygon_with_holes(in_file.read())
assert(pwh.outer_boundary().is_counterclockwise_oriented())
for hole in pwh.holes():
  assert(hole.is_clockwise_oriented())
  assert(hole.is_simple())
Pol2.draw(pwh)

offset_polygons = Sn2.create_interior_skeleton_and_offset_polygons_with_holes_2(offset, pwh)
try:
  print(f"Offsetting: {offset}")
  offset_polygons = Sn2.create_interior_skeleton_and_offset_polygons_with_holes_2(offset, pwh)
  print(f"Result: {eps_filename}")
  dump_to_eps(pwh, offset_polygons, eps_filename)
except:
  print("ERROR: creating interior straight skeleton")
