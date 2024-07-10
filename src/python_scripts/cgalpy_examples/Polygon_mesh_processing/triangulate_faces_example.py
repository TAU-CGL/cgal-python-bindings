import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[3]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = "meshes/P.off" if len(sys.argv) < 2 else sys.argv[1]
outfilename = "P_tri.off" if len(sys.argv) < 3 else sys.argv[2]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    raise ValueError("Error: Invalid input.")
if mesh.is_empty():
    raise ValueError("Warning: empty file?")
if not mesh.is_triangle_mesh():
    print("Input mesh is not triangulated.")

try:
    triangulated_mesh = Pmp.triangulate_faces(mesh)
except:
    raise ValueError("Error: Could not triangulate faces.")

# Confirm that all faces are triangles.
faces = mesh.faces()
for f in faces:
    he = Sm.halfedge(f, triangulated_mesh)
    if not Sm.is_triangle(he, triangulated_mesh):
        raise ValueError("Error: non-triangular face left in mesh.")

Sm.write_polygon_mesh(outfilename, triangulated_mesh)


