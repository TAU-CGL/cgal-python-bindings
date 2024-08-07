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
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/P.off')

try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

eif = mesh.add_property_map_edge_bool("e:is_feature")[0]
pid = mesh.add_property_map_face_int("f:patch_id")[0]
vip = mesh.add_property_map_vertex_set_int("v:incident_patches")[0]

number_of_patches = Pmp.sharp_edges_segmentation(mesh, 90, eif, pid,
                                                 {"vertex_incident_patches_map": vip})

nb_sharp_edges = 0
for e in mesh.edges():
  if Sm.get(eif, e):
    nb_sharp_edges += 1

print(f"This mesh contains {nb_sharp_edges} sharp edges")
print(f" and {number_of_patches} surface patches.")
