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
Bso2 = CGALPY.Bso2

# #include "pgn_print.h"

# Constuct two triangular polygons and check whether they intersect.
T1 = Polygon([Point(1, 1), Point(4, 1), Point(1, 3)])
print("T1 = ", T1)
T2 = Polygon([Point(2, 2), Point(5, 1), Point(4, 3)])
print("T2 = ", T2)
if Bso2.do_intersect(T1, T2):
  print("The two polygons intersect in their interior.")
else:
  print("The two polygons do not intersect.")
