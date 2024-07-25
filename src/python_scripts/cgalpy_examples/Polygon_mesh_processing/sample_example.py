#!/usr/bin/python
import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = 'meshes/eight.off' if len(sys.argv) < 2 else sys.argv[1]

try:
  mesh = Sm.read_polygon_mesh(filename)
except:
  print("Invalid input.")
  exit(1)

points_per_face = 10 if len(sys.argv) < 2 else int(sys.argv[1])

points = Pmp.sample_triangle_mesh(mesh, {"number_of_points_per_face": points_per_face})

point_set = set(Pmp.sample_triangle_mesh(mesh))

print(len(point_set))
