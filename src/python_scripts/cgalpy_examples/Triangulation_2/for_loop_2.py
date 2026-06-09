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
Tri2 = CGALPY.Tri2

triangulation = Tri2.Triangulation_2()
for point in [Ker.Point_2(0, 0), Ker.Point_2(1, 0), Ker.Point_2(0, 1)]:
    triangulation.insert(point)

print("Triangulation_2::Finite_vertices_iterator is like a  Triangulation_2::Vertex_handle")
for vertex in triangulation.finite_vertices():
    print(triangulation.point(vertex))

print("Triangulation_2::Finite_vertex_handles::iterator dereferences to Triangulation_2::Vertex_handle")
for vertex in triangulation.finite_vertices():
    print(triangulation.point(vertex))

print("and you can use a C++11 for loop")
for vertex in triangulation.finite_vertices():
    print(triangulation.point(vertex))
