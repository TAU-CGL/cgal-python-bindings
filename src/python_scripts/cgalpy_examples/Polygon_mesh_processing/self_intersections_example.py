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
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))

CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/pig.off")
try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

print("Using parallel mode? 0")

timer = time.time()
intersecting = Pmp.does_self_intersect(mesh)

print("There are self-intersections." if intersecting else "There is no self-intersection.")
print(f"Elapsed time (does self intersect): {(time.time() - timer):.7f}")

timer = time.time()

intersected_tris = Pmp.self_intersections(mesh)
print(f"{len(list(intersected_tris))} pairs of triangles intersect.")
print(f"Elapsed time (self intersections): {(time.time() - timer):.7f}")
