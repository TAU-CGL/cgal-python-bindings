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
Tri3 = CGALPY.Tri3

Point = Ker.Point_3
triangulation = Tri3.Delaunay_triangulation_3()

for point in [
    Point(0, 0, 0),
    Point(1, 0, 0),
    Point(0, 1, 0),
    Point(0, 1, 1),
]:
    triangulation.insert(point)

print("Triangulation_3::Finite_vertices_iterator is like a  Triangulation_3::Vertex_handle")
for vertex in triangulation.finite_vertices():
    print(triangulation.point(vertex))

print("Triangulation_3::Finite_vertex_handles::iterator dereferences to Triangulation_3::Vertex_handle")
for vertex in triangulation.finite_vertices():
    print(triangulation.point(vertex))

print("and you can use a C++11 for loop")
for vertex in triangulation.finite_vertices():
    print(triangulation.point(vertex))
