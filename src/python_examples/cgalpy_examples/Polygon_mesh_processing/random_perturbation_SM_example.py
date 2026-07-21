#!/usr/bin/python

import os
import sys
import importlib

lib = 'CGALPY'
i = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    i = 2

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off")
i += 1
max_size = sys.argv[i] if len(sys.argv) < i else 0.02

try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

mesh.points()
Pmp.random_perturbation(mesh, max_size, {"vertex_point_map": mesh.points()})
Sm.write_polygon_mesh("data/eight_perturbed.off", mesh, {"stream_precision": 17})
