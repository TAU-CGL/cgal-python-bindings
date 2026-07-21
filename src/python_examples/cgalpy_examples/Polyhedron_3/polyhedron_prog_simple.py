#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Polyhedron = Pol3.Polyhedron_3
Ker = CGALPY.Ker
Point = Ker.Point_3

P1 = Polyhedron()
p1 = Point(0,1,0)
p2 = Point(0,0,0)
p3 = Point(1,1,0)
p4 = Point(1,0,0)
h1 = P1.make_tetrahedron(p1, p2, p3, p4);
if P1.is_tetrahedron(h1): print("true")
else: print("false")

P2 = Polyhedron()
h2 = P2.make_tetrahedron();
if P2.is_tetrahedron(h2): print("true")
else: print("false")
