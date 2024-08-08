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
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename1 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/blobby.off")
i += 1
filename2 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off")

try: mesh1 = Sm.read_polygon_mesh(filename1)
except: raise ValueError("Invalid input 1.")

try: mesh2 = Sm.read_polygon_mesh(filename2)
excep: raise ValueError("Invalid input 2.")

polylines = Pmp.surface_intersection(mesh1, mesh2)

#### WORK IN PROGRESS
