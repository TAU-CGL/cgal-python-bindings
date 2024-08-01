import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename = "meshes/P.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Invalid input.")
    exit(1)

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

