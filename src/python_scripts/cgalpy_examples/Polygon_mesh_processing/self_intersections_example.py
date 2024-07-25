#!/usr/bin/python
import time
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

filename = 'meshes/pig.off' if len(sys.argv) < 2 else sys.argv[1]

mesh = Sm.read_polygon_mesh(filename)

print("Using parallel mode? 0")

timer = time.time()
intersecting = Pmp.does_self_intersect(mesh)

print("There are self-intersections." if intersecting else "There is no self-intersection.")
print(f"Elapsed time (does self intersect): {(time.time() - timer):.7f}")

timer = time.time()

intersected_tris = Pmp.self_intersections(mesh)
print(f"{len(list(intersected_tris))} pairs of triangles intersect.")
print(f"Elapsed time (self intersections): {(time.time() - timer):.7f}")
