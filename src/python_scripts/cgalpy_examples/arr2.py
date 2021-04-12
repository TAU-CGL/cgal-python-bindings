#!/usr/bin/python3.9
# export PYTHONPATH=...

import sys
import importlib

if len(sys.argv) < 2:
    print('Library name missing, assuming CGALPY')
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Arr2 = CGALPY.Arr2

Arrangement_2 = Arr2.Arrangement_2
Point_2 = Ker.Point_2
Curve_2 = Arr2.Curve_2

arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(2, 0))
c2 = Curve_2(Point_2(1, 2), Point_2(1, -2))
c3 = Curve_2(Point_2(0, 0), Point_2(1, 2))
c4 = Curve_2(Point_2(1, -2), Point_2(2, 0))
Arr2.insert(arr, [c1, c2, c3, c4])
print("Number of faces in the arrangement:", arr.number_of_faces())
print("Number of halfedges in the arrangement:", arr.number_of_halfedges())
print("Number of vertices in the arrangement:", arr.number_of_vertices())

# Iteration example
v: Arr2.Vertex
for v in arr.vertices():
    print(v.point())

e: Arr2.Halfedge
for e in arr.halfedges():
    print(e.curve())

f: Arr2.Face
for f in arr.faces():
    print(f.is_unbounded())
