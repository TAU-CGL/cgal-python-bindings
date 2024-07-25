#!/usr/bin/python
import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

filename = 'meshes/eight.off' if len(sys.argv) < 2 else sys.argv[1]

mesh = Pol3.read_polygon_mesh(filename)

face_normal, vertex_normal = Pmp.compute_normals(mesh) # normals = ((face_handle, Vector), (vertex_handle, Vector))

print("Face normals :")
for n in face_normal[1]:
  print(n)

print("Vertex normals :")
for n in vertex_normal[1]:
  print(n)
