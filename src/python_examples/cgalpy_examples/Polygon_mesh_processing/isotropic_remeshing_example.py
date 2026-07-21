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
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

def halfedge2edge(mesh, edges):
  return [Sm.edge(h, mesh) for h in edges]

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/pig.off")
i += 1
target_edge_length = float(sys.argv[i]) if len(sys.argv) > i else 0.04
i += 1
nb_iter = int(sys.argv[i]) if len(sys.argv) > i else 10

mesh = Sm.read_polygon_mesh(filename)

print("Split border...", end="")

border = halfedge2edge(mesh, Pmp.border_halfedges(Sm.faces(mesh), mesh))
Pmp.split_long_edges(border, target_edge_length, mesh)

print("done.")
print(f"Start remeshing of {filename} ({Sm.num_faces(mesh)} faces)...")
Pmp.isotropic_remeshing(Sm.faces(mesh), target_edge_length, mesh,
                        {"number_of_iterations": nb_iter, "protect_constraints": True})

Sm.write_polygon_mesh("out.off", mesh, {"stream_precision": 17})

print("Remeshing done.")
