#!/usr/bin/python
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>


import importlib
import sys

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm

mesh = Sm.Surface_mesh_3()

u = mesh.add_vertex(Ker.Point_3(0, 1, 0))
v = mesh.add_vertex(Ker.Point_3(0, 0, 0))
w = mesh.add_vertex(Ker.Point_3(1, 0, 0))
x = mesh.add_vertex(Ker.Point_3(1, 1, 0))

mesh.add_face(u, v, w, x)


def vertex_id(vertex):
    return f"v{vertex}"


print("all vertices ")
for vertex in mesh.vertices():
    print(f"{vertex_id(vertex)} {mesh.point(vertex)}")

for vertex in mesh.vertices():
    print(vertex_id(vertex))

for vertex in mesh.vertices():
    print(vertex_id(vertex))
