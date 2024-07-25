#!/usr/bin/python
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
Point_3 = Ker.Point_3
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

filename = "meshes/quads_to_stitch.off" if len(sys.argv) < 2 else sys.argv[1]

try:
  mesh = Pol3.read_polygon_mesh(filename)
except:
  print("Invalid input.")
  exit(1)

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
