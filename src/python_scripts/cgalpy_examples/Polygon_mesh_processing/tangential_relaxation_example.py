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

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/pig.off"
i += 1
nb_iter = int(sys.argv[i]) if len(sys.argv) < i else 10

try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

if not Sm.is_triangle_mesh(mesh):
  raise ValueError("Invalid input.")

print("Relax...", end='')

Pmp.tangential_relaxation(mesh, {'number_of_iterations': nb_iter})

print("done.")
