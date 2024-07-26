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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = "meshes/eight.off" if len(sys.argv) < 2 else sys.argv[1]

try:
  mesh = Sm.read_polygon_mesh(filename)
except:
  print("Invalid input.")
  exit(1)

max_size = 0.02 if len(sys.argv) < 3 else sys.argv[2]

mesh.points()
Pmp.random_perturbation(mesh, max_size, {"vertex_point_map": mesh.points()})

Sm.write_polygon_mesh("data/eight_perturbed.off", mesh, {"stream_precision": 17})

