#!/usr/bin/python
import os
import sys
import importlib
if len(sys.argv) < 5:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[4]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

def halfedge2edge(mesh, edges):
  return [Sm.edge(h, mesh) for h in edges]

filename = "meshes/pig.off" if len(sys.argv) < 2 else sys.argv[1]

mesh = Sm.read_polygon_mesh(filename)

target_edge_length = 0.04 if len(sys.argv) < 3 else float(sys.argv[2])
nb_iter = 10 if len(sys.argv) < 4 else int(sys.argv[3])

print("Split border...", end="")

border = halfedge2edge(mesh, Pmp.border_halfedges(Sm.faces(mesh), mesh))
Pmp.split_long_edges(border, target_edge_length, mesh)

print("done.")
print(f"Start remeshing of {filename} ({Sm.num_faces(mesh)} faces)...")
Pmp.isotropic_remeshing(Sm.faces(mesh), target_edge_length, mesh,
                        {"number_of_iterations": nb_iter, "protect_constraints": True})

Sm.write_polygon_mesh("out.off", mesh, {"stream_precision": 17})

print("Remeshing done.")
