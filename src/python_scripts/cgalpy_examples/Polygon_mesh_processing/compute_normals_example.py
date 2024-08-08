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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off")

mesh = Sm.read_polygon_mesh(filename)

face_normal, vertex_normal = Pmp.compute_normals(mesh) # normals = ((face_handle, Vector), (vertex_handle, Vector))

print("Vertex normals :")
for n in vertex_normal[1]:
  print(n)

print("Face normals :")
for n in face_normal[1]:
  print(n)
