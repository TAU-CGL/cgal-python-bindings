#!/usr/bin/python
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

filename1 = "meshes/blobby.off" if len(sys.argv) < 2 else sys.argv[1]
filename2 = "meshes/eight.off" if len(sys.argv) < 3 else sys.argv[2]

try:
  mesh1 = Sm.read_polygon_mesh(filename1)
  mesh2 = Sm.read_polygon_mesh(filename2)
except:
  print("Invalid input.")
  exit(1)

polylines = Pmp.surface_intersection(mesh1, mesh2)

#### WORK IN PROGRESS
