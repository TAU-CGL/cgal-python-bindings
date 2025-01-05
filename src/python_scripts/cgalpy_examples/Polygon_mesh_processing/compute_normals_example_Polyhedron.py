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
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off")
mesh = Pol3.read_polygon_mesh(filename)

print("Face normals :")
for f in Pol3.faces(mesh):
    print(Pmp.compute_face_normal(f, mesh))

print("Vertex normals :")
for f in Pol3.vertices(mesh):
    print(Pmp.compute_vertex_normal(f, mesh))
