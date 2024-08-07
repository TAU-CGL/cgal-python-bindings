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

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/blobby_3cc.off"))
try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

fccmap = mesh.add_property_map_face_size_t("f:CC")[0]
num = Pmp.connected_components(mesh, fccmap)
sys.stderr.write(f"- The graph has {num} connected components (face connectivity)\n")

print("The faces in component 0 are:")
ffg = Sm.Face_filtered_graph(mesh, 0, fccmap)
for f in mesh.faces(): print(f"f{f}")

if num > 1:
  components = [0, 1]
  print("The faces in components 0 and 1 are:")
  ffg.set_selected_faces(components, fccmap)
  for f in Sm.faces(ffg): print(f"f{f}")
