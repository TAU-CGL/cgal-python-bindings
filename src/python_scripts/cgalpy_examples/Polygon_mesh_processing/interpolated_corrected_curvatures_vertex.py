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
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else "meshes/sphere.off"

mesh = Sm.read_polygon_mesh(filename)

for v in Sm.vertices(mesh):
  h, g, p = Pmp.interpolated_corrected_curvatures(v, mesh)
  print(f"{v.id()}: HC = {h}, GC = {g}, PC = [ {p.min_curvature} , {p.max_curvature} ]")
