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
Segment_2 = Ker.Segment_2
Curve_2 = Aos2.Curve_2

arr = Arrangement_2()
p0 = Point_2(0, 0)
p1 = Point_2(1, 0)
p2 = Point_2(1, 1)
p3 = Point_2(0, 1)

Aos2.insert(arr, Curve_2(Segment_2(p0, p1)))
Aos2.insert(arr, Curve_2(Segment_2(p1, p2)))
Aos2.insert(arr, Curve_2(Segment_2(p2, p3)))
Aos2.insert(arr, Curve_2(Segment_2(p3, p0)))

res = []
c = Curve_2(Segment_2(Point_2(0.5, 0.5), Point_2(1.5, 0.5)))
Aos2.zone(arr, c, res)
# res contains the objects of the arrangement that the curve intersects
# in the order when going from left to right
for obj in res:
    if obj.is_vertex():
        v = Aos2.Vertex()
        obj.get_vertex(v)
        print("vertex")
    elif obj.is_halfedge():
        he = Aos2.Halfedge()
        obj.get_halfedge(he)
        print("halfedge")
    elif obj.is_face():
        f = Aos2.Face()
        obj.get_face(f)
        print("face")
