#!/usr/bin/python3.7
# export PYTHONPATH=...
# import os
# import sys
# import importlib

# if len(sys.argv) < 2:
#     sys.path.append(os.path.abspath('../precompiled'))
#     lib = 'CGALPY'
# else:
#     lib = sys.argv[1]

# CGALPY = importlib.import_module(lib)
import CGALPY
Ker = CGALPY.Ker
Arr2 = CGALPY.Arr2

Arrangement_2 = Arr2.Arrangement_2
Point_2 = Ker.Point_2
Curve_2 = Arr2.Curve_2
print(type(Curve_2))
arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(1, 0))
c2 = Curve_2(Point_2(1, 0), Point_2(0, 1))
c3 = Curve_2(Point_2(0, 1), Point_2(0, 0))

# Inserting the curves into the arrangement
Arr2.insert(arr, [c1, c2, c3])
print("Number of vertices in the arrangement:", arr.number_of_vertices())
print("Number of halfedges in the arrangement:", arr.number_of_halfedges())
print("Number of faces in the arrangement:", arr.number_of_faces())

unb_face = arr.unbounded_face()
unb_face.set_data(1)
print(type(unb_face.inner_ccbs()))
print(type(next(unb_face.inner_ccbs())))
print(type(next(next(unb_face.inner_ccbs()))))
next(next(unb_face.inner_ccbs())).twin().face().set_data(0)
for f in arr.faces():
  print(f.is_unbounded(), f.data())
