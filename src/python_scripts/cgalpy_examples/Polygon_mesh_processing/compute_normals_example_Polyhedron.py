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
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else 'meshes/eight.off'
mesh = Pol3.read_polygon_mesh(filename)
face_normal, vertex_normal = Pmp.compute_normals(mesh) # normals = ((face_handle, Vector), (vertex_handle, Vector))

print("Face normals :")
for n in face_normal[1]: print(n)

print("Vertex normals :")
for n in vertex_normal[1]: print(n)
