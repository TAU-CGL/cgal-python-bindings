#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Bso2 = CGALPY.Bso2
Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Polygon_with_holes = Pol2.Polygon_with_holes_2
ppoints = [Point(0, 0), Point(9, 0), Point(9, 2), Point(7, 4), Point(4.5, 1.5), Point(2, 4), Point(0, 2)]
P = Polygon(ppoints)
print(P)
qpoints = [Point(0, 6), Point(0, 4), Point(2, 2), Point(4.5, 4.5), Point(7, 2), Point(9, 4), Point(9, 6)]
Q = Polygon(qpoints)
print(Q)
unionR = Polygon_with_holes()
res = Bso2.join(P, Q, unionR)
if res: print(unionR)
else: print("P and Q are disjoint and their union is trivial.")
intersectionR = Bso2.intersection(P, Q)
print(intersectionR)
