#!/usr/bin/python3

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
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename1 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/blobby.off")
i += 1
filename2 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off")

try: mesh1 = Sm.read_polygon_mesh(filename1)
except: raise ValueError("Invalid input 1.")

try: mesh2 = Sm.read_polygon_mesh(filename2)
except: raise ValueError("Invalid input 2.")

print(f"Number of vertices before corefinement {Sm.num_vertices(mesh1)} and {Sm.num_vertices(mesh2)}")

Pmp.corefine(mesh1, mesh2)

print(f"Number of vertices after corefinement {Sm.num_vertices(mesh1)} and {Sm.num_vertices(mesh2)}")

Sm.write_polygon_mesh("mesh1_refined.off", mesh1, {"stream_precision": 17})
Sm.write_polygon_mesh("mesh2_refined.off", mesh2, {"stream_precision": 17})
