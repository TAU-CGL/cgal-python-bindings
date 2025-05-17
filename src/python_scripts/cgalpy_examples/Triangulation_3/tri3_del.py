#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Tri3 = CGALPY.Tri3
Point_3 = Ker.Point_3
p1 = Point_3(0, 0, 0)
p2 = Point_3(1, 0, 0)
p3 = Point_3(0, 1, 0)
p4 = Point_3(0, 0, 1)
p5 = Point_3(2, 2, 2)
p6 = Point_3(-1, 0, 1)
dt = Tri3.Delaunay_triangulation_3([p1, p2, p3, p4, p5, p6])
print(dt.is_valid(False, 0))

for c in dt.finite_cells():
  tet = dt.tetrahedron(c)
  print(tet.volume())
