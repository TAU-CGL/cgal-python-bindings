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
Ch2 = CGALPY.Ch2

Point_2 = Ker.Point_2

points = [
    Point_2(0, 0),
    Point_2(10, 0),
    Point_2(10, 10),
    Point_2(6, 5),
    Point_2(4, 1),
]

result = Ch2.convex_hull_2(points)
print(f"{len(result)} points on the convex hull:")
for point in result:
    print(point)
