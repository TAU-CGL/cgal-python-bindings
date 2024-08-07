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
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/elephant.off"))
try: input_points, input_triangles = Sm.read_polygon_soup(filename)
except: raise ValueError("Invalid input.")

input_points, input_triangles = Pmp.repair_polygon_soup(input_points, input_triangles)
input_points, input_triangles = Pmp.triangulate_polygons(input_points, input_triangles)

t = time.perf_counter()
input_points, input_triangles = Pmp.autorefine_triangle_soup(input_points, input_triangles)
t2 = time.perf_counter()
print(f"#points = {len(input_points)} and #triangles = {len(input_triangles)} in {(t2 - t):.6f} sec.")
