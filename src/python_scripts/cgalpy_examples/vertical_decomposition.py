#!/usr/bin/python3.9
# export PYTHONPATH=...

import os
import sys
import importlib

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Aos2 = CGALPY.Aos2

Arrangement_2 = Aos2.Arrangement_2
Point_2 = Ker.Point_2
Curve_2 = Aos2.Curve_2

arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(2, 0))
c2 = Curve_2(Point_2(2, 0), Point_2(2, 2))
c3 = Curve_2(Point_2(2, 2), Point_2(0, 2))
c4 = Curve_2(Point_2(0, 2), Point_2(0, 0))

Aos2.insert(arr, [c1, c2, c3, c4])

Aos2.insert_point(arr, Point_2(1, 1))

d = []

Aos2.decompose(arr, d)
for pair in d:
    # CGALPY specific:
    # pair is a tuple
    # pair[0] is an arrangement vertex
    # pair[1] is a pair holding the objects (Vertex, Halfedge, or Face) above and below
    # the vertex(respectively), that is, the objects hit by the vertical walls emanating from the vertex
    v0 = pair[0]
    for obj in pair[1]:
        if obj.is_vertex():
            v1 = Aos2.Vertex()
            obj.get_vertex(v1)
        elif obj.is_halfedge():
            he = Aos2.Halfedge()
            obj.get_halfedge(he)
        elif obj.is_face():
            f = Aos2.Face()
            obj.get_face(f)
