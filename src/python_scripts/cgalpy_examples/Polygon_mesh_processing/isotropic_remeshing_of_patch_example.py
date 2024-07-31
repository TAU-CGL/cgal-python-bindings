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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

def halfedge2edge(mesh, edges):
    return [Sm.edge(h, mesh) for h in edges]

filename = "meshes/pig.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Invalid input.")
    sys.exit(1)

if not Sm.is_triangle_mesh(mesh):
    print("Invalid input.")
    sys.exit(1)

target_edge_length = 0.1
nb_iter = 3

print("Split border...", end="")

border = Pmp.border_halfedges(Sm.faces(mesh), mesh)

Pmp.split_long_edges(halfedge2edge(mesh, border), target_edge_length, mesh)

print("done.")
print(f"Start remeshing of {filename} ({Sm.num_faces(mesh)} faces,", end="")

selected = mesh.add_property_map_face_int("f:selected", 0)[0]

seed = [Sm.faces(mesh)[0]]
selected[seed[0]] = True

patch = Sm.expand_face_selection(seed, mesh, 5, selected)

print(f" and patch of size {len(patch)}")
Pmp.isotropic_remeshing(patch, target_edge_length, mesh,
                        {"number_of_iterations": nb_iter,
                         "face_patch_map": selected,
                         "protect_constraints": True})

Sm.write_polygon_mesh("out.off", mesh, {"stream_precision": 17})
print("Remeshing done.")

