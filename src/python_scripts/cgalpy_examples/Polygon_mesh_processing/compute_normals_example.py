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

vertex_normal_map = mesh.add_property_map_vertex_vector("vn")[0]
face_normal_map = mesh.add_property_map_face_vector("fn")[0]

Pmp.compute_normals(mesh, vertex_normal_map, face_normal_map)

print("Vertex normals :")
for v in mesh.vertices():
    print(vertex_normal_map[v])

print("Face normals :")
for f in mesh.faces():
    print(face_normal_map[f])

