#!/usr/bin/python3

import os
import sys
import importlib

if len(sys.argv) < 3:
  print("Usage: intersection.py [CGALPY[_str]] <file1.off> <file2.off> [lib]")
  exit()

i = 1
if len(sys.argv) < 4:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]
  i = 2

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename1 = sys.argv[i]
filename2 = sys.argv[i+1]

sm1 = Sm.read_polygon_mesh(filename1)
sm2 = Sm.read_polygon_mesh(filename1)
rc1 = Pmp.does_self_intersect(sm1)
print(rc1)
rc2 = Pmp.do_intersect(sm1, sm2)
print(rc2)
