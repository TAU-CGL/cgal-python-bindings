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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename1 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/blobby.off")
i += 1
filename2 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off")

try: mesh1 = Sm.read_polygon_mesh(filename1)
except: raise ValueError("Invalid input.")
Sm.draw(mesh1)

try: mesh2 = Sm.read_polygon_mesh(filename2)
except: raise ValueError("Invalid input.")
Sm.draw(mesh2)

try:
  out = Pmp.corefine_and_compute_union(mesh1, mesh2, {'visitor': Pmp.Corefine_visitor()})
except:
  raise ValueError("Cannot compute union!")
Sm.draw(out)

print("Union was successfully computed")
Sm.write_polygon_mesh("union.off", out);
