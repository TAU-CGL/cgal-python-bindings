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

for j in range(i, len(sys.argv)):
  filename = sys.argv[j]
  try: mesh = Sm.read_polygon_mesh(filename)
  except:
    print(f"Invalid input: {sys.argv[j]}")
    continue
  if not Sm.is_triangle_mesh(mesh):
    print(filename)
