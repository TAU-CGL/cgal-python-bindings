#!/usr/bin/python3.7

from arr2_epic_seg import *
arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(2, 0))
c2 = Curve_2(Point_2(1, 2), Point_2(1, -2))
c3 = Curve_2(Point_2(0, 0), Point_2(1, 2))
c4 = Curve_2(Point_2(1, -2), Point_2(2, 0))
insert(arr, [c1, c2, c3, c4])
print("Number of faces in the arrangement:", arr.number_of_faces())
print("Number of halfedges in the arrangement:", arr.number_of_halfedges())
print("Number of vertices in the arrangement:", arr.number_of_vertices())

# Iteration example
for v in arr.vertices():
    print(v.point())
