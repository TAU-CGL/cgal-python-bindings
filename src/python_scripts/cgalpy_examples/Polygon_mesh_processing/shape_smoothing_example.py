#!/usr/bin/python

import time
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
Point_3 = Ker.Point_3
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/pig.off")
i += 1
nb_iterations = int(sys.argv[i]) if len(sys.argv) > i else 1000
i += 1
time = float(sys.argv[i]) if len(sys.argv) > i else 0.0001

try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

vcmap, success = mesh.add_property_map_vertex_bool("v:bool")
if not success:
  raise ValueError("Failed to add property map.")

constrained_vertices = 0

for v in Sm.vertices(mesh):
  if Sm.is_border(v, mesh):
    vcmap[v] = True
    constrained_vertices += 1

print(f"Constraining: {constrained_vertices} border vertices")

print(f"Smoothing shape... ({nb_iterations} iterations)")
Pmp.smooth_shape(mesh, time, {"number_of_iterations": nb_iterations, "vertex_is_constrained_map": vcmap})

Sm.write_polygon_mesh("mesh_shape_smoothed.off", mesh, {"stream_precision": 17})

print("Done!")
