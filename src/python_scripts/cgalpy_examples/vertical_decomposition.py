#!/usr/bin/python3.7
# export PYTHONPATH=...

import sys
import importlib

if len(sys.argv) < 2:
  sys.exit('Library name missing')

Arr2 = importlib.import_module(sys.argv[1]+".Arr2")
Ker = importlib.import_module(sys.argv[1]+".Ker")

Arrangement_2 = Arr2.Arrangement_2
Point_2 = Ker.Point_2
Curve_2 = Arr2.Curve_2

arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(2, 0))
c2 = Curve_2(Point_2(2, 0), Point_2(2, 2))
c3 = Curve_2(Point_2(2, 2), Point_2(0, 2))
c4 = Curve_2(Point_2(0, 2), Point_2(0, 0))

Arr2.insert(arr, [c1, c2, c3, c4])

Arr2.insert_point(arr, Point_2(1, 1))

d = []

Arr2.decompose(arr, d)
for pair in d:
    # pair is a tuple
    # pair[0] is an arrangement vertex
    # pair[1] is a pair holding the objects (Vertex, Halfedge, or Face) above and below
    # the vertex(respectively), that is, the objects hit by the vertical walls emanating from the vertex
    v0 = pair[0]
    for obj in pair[1]:
        if obj.is_vertex():
            v1 = Arr2.Vertex()
            obj.get_vertex(v1)
        elif obj.is_halfedge():
            he = Arr2.Halfedge()
            obj.get_halfedge(he)
        elif obj.is_face():
            f = Arr2.Face()
            obj.get_face(f)
