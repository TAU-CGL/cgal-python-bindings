#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point = Ker.Point_3
Tri3 = CGALPY.Tri3
dt = Tri3.Triangulation_3()
dt.insert(Point(1,0,0));
dt.insert(Point(0,1,0));
dt.insert(Point(0,0,1));
dt.insert(Point(2,2,2));
dt.insert(Point(-1,0,1));
Tri3.draw(dt, "3D Triangulation")

for v in dt.finite_vertices(): v.set_info(dt.degree(v))
for v in dt.finite_vertices(): print(v.info())
