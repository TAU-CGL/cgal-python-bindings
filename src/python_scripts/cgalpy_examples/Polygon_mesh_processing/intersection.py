#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 4:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[3]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

try:
  filename1 = sys.argv[1]
  filename2 = sys.argv[2]

except Exception:
  print("Usage: intersection.py <file1.off> <file2.off>")
  exit()

sm1 = Sm.read_polygon_mesh(filename1)
sm2 = Sm.read_polygon_mesh(filename1)
rc1 = Pmp.does_self_intersect(sm1)
print(rc1)
rc2 = Pmp.do_intersect_meshes(sm1, sm2)
print(rc2)
