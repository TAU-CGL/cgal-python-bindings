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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/sphere.off")
try:
  mesh = Pmp.read_polygon_mesh(filename)
except:
  raise ValueError("Invalid input.")

for i in range(Sm.num_vertices(mesh)):
  v = Sm.Vertex_index(i)
  h, g, p = Pmp.interpolated_corrected_curvatures(v, mesh)
  print(f"{v.id()}: HC = {h}, GC = {g}, PC = [ {p.min_curvature} , {p.max_curvature} ]")
