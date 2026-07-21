#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]
CGALPY = importlib.import_module(lib)

Aos2 = CGALPY.Aos2
Arrangement_2 = Aos2.Arrangement_2
Point_2 = Arrangement_2.Geometry_traits_2.Point_2
Curve_2 = Arrangement_2.Geometry_traits_2.Curve_2
arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(1, 0))
c2 = Curve_2(Point_2(1, 0), Point_2(0, 1))
c3 = Curve_2(Point_2(0, 1), Point_2(0, 0))

# Insert the curves into the arrangement
Aos2.insert(arr, [c1, c2, c3])
print("Number of vertices in the arrangement:", arr.number_of_vertices())
print("Number of halfedges in the arrangement:", arr.number_of_halfedges())
print("Number of faces in the arrangement:", arr.number_of_faces())

unb_face = arr.unbounded_face()
unb_face.set_data(0)
next(next(unb_face.inner_ccbs())).twin().face().set_data(1)
for f in arr.faces(): print(f.is_unbounded(), f.data())
