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
Pol3 = CGALPY.Pol3

points = [
    Ker.Point_3(1, 0, 0),
    Ker.Point_3(0, 1, 0),
    Ker.Point_3(0, 0, 1),
    Ker.Point_3(0, 0, 0),
]

polyhedron = Pol3.Polyhedron_3()
polyhedron.make_tetrahedron(points[0], points[1], points[2], points[3])

for point in polyhedron.points():
    print(point)
