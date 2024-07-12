import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = 'meshes/eight.off' if len(sys.argv) < 2 else sys.argv[1]

mesh = Sm.read_polygon_mesh(filename)

normals = Pmp.compute_normals(mesh) # normals = ((face_handle, Vector), (vertex_handle, Vector))
face_normal = normals[0]
vertex_normal = normals[1]

print("Vertex normals :")
for n in vertex_normal[1]:
    print(n)

print("Face normals :")
for n in face_normal[1]:
    print(n)

