#!/usr/bin/python3

import os
import sys
import importlib
import numpy as np

lib = 'CGALPY'
filename1 = 'meshes/cube.off'
filename2 = 'meshes/P.off'
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    if len(sys.argv) > 2: filename1 = sys.argv[2]
    if len(sys.argv) > 3: filename2 = sys.argv[3]
  else:
    filename1 = str
    if len(sys.argv) > 2: filename2 = sys.argv[2]
    if len(sys.argv) > 3: lib = sys.argv[3]

CGALPY = importlib.import_module(lib)
fullname1 = CGALPY.data_file_path(filename1)
fullname2 = CGALPY.data_file_path(filename2)

Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

sm1 = Pmp.read_polygon_mesh(fullname1)
sm2 = Pmp.read_polygon_mesh(fullname2)
rc1 = Pmp.does_self_intersect(sm1)
print(rc1)
rc2 = Pmp.do_intersect(sm1, sm2)
print(rc2)

polyline1 = np.array([[0.0, 0.0, 0.0], [1.0, 1.0, 0.0]], dtype=np.float64)
polyline2 = np.array([[0.0, 1.0, 0.0], [1.0, 0.0, 0.0]], dtype=np.float64)
rc3 = Pmp.do_intersect(polyline1, polyline2)
print(rc3)

mesh_polyline = np.array([[-1.0, 0.5, 0.5], [2.0, 0.5, 0.5]], dtype=np.float64)
rc4 = Pmp.do_intersect(sm1, mesh_polyline)
print(rc4)
