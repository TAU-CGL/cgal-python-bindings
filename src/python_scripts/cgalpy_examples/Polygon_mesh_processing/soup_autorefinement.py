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
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename = "meshes/elephant.off" if len(sys.argv) < 2 else sys.argv[1]

try:
  input_points, input_triangles = Sm.read_polygon_soup(filename)
except:
  print(f"Cannot read {filename}")
  exit(1)

input_points, input_triangles = Pmp.repair_polygon_soup(input_points, input_triangles)
input_points, input_triangles = Pmp.triangulate_polygons(input_points, input_triangles)

t = time.perf_counter()
input_points, input_triangles = Pmp.autorefine_triangle_soup(input_points, input_triangles)
t2 = time.perf_counter()
print(f"#points = {len(input_points)} and #triangles = {len(input_triangles)} in {(t2 - t):.6f} sec.")
