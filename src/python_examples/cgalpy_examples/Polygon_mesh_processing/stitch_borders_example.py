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
Point_3 = Ker.Point_3
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/quads_to_stitch.off"
try: mesh = Pol3.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

print("Before stitching : ")
print(f"\t Number of vertices  :\t{mesh.size_of_vertices()}")
print(f"\t Number of halfedges :\t{mesh.size_of_halfedges()}")
print(f"\t Number of facets    :\t{mesh.size_of_facets()}")

Pmp.stitch_borders(mesh)

print("Stitching done : ")
print(f"\t Number of vertices  :\t{mesh.size_of_vertices()}")
print(f"\t Number of halfedges :\t{mesh.size_of_halfedges()}")
print(f"\t Number of facets    :\t{mesh.size_of_facets()}")

Pol3.write_polygon_mesh("mesh_stitched.off", mesh, {"stream_precision": 17})
