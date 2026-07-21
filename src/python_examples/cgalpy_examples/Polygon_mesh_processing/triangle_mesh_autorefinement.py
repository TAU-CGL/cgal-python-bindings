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
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/elephant.off"))
try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")
Pmp.autorefine(mesh)
Sm.write_polygon_mesh("autorefined.off", mesh, {"stream_precision": 17})
