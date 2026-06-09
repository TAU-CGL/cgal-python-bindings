#!/usr/bin/python

import importlib
import sys
import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)

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

face = mesh.add_face(u, v, w, x)


def vertex_id(vertex):
    return f"v{vertex}"


def face_id(face_index):
    return f"f{face_index}"


print(f"vertices around vertex {vertex_id(v)}")
for vertex in Sm.vertices_around_target(mesh.halfedge(v), mesh):
    print(vertex_id(vertex))

print(f"vertices around face {face_id(face)}")
for vertex in Sm.vertices_around_face(mesh.halfedge(face), mesh):
    print(vertex_id(vertex))

for vertex in Sm.vertices_around_face(mesh.halfedge(face), mesh):
    print(vertex_id(vertex))
