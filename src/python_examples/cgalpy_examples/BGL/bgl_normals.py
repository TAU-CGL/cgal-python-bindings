#!/usr/bin/python

# Author: Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import importlib
import sys

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

Point_3 = Ker.Point_3

mesh = Sm.Surface_mesh_3()

points = [
    Point_3(-1, -1,  1),
    Point_3(-1,  1,  1),
    Point_3( 1,  1,  1),
    Point_3( 1, -1,  1),
    Point_3(-1, -1, -1),
    Point_3(-1,  1, -1),
    Point_3( 1,  1, -1),
    Point_3( 1, -1, -1),
]

vertices = [mesh.add_vertex(point) for point in points]

# Exact face order/orientation from CGAL Data/data/meshes/cube_poly.off.
mesh.add_face(vertices[3], vertices[2], vertices[1])
mesh.add_face(vertices[3], vertices[1], vertices[0])
mesh.add_face(vertices[0], vertices[1], vertices[5], vertices[4])
mesh.add_face(vertices[6], vertices[5], vertices[1], vertices[2])
mesh.add_face(vertices[3], vertices[7], vertices[6], vertices[2])
mesh.add_face(vertices[4], vertices[7], vertices[3], vertices[0])
mesh.add_face(vertices[4], vertices[5], vertices[6], vertices[7])

print("Normals")
for face in mesh.faces():
    print(Pmp.compute_face_normal(face, mesh))
