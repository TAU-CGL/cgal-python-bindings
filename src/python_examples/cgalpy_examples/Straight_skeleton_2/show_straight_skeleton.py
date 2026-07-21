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

# Read a polygon with holes.
try: filename = sys.argv[2]
except: filename = 'sample_1.dat'
try: eps_filename = sys.argv[3]
except: eps_filename = filename + ".skeleton.eps"

in_file = open(filename, 'r')
pwh = Polygon_with_holes(in_file.read())
assert(pwh.outer_boundary().is_counterclockwise_oriented())
for hole in pwh.holes():
  assert(hole.is_clockwise_oriented())
  assert(hole.is_simple())
Pol2.draw(pwh)

try:
  ss = Sn2.create_interior_straight_skeleton_2(pwh)
  print(f"Result: {eps_filename}")
  dump_to_eps(pwh, ss, eps_filename)
except:
  print("ERROR: creating interior straight skeleton")
