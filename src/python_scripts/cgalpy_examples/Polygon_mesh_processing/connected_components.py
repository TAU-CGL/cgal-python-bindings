#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

p1 = Point_3(0, 0, 0)
p2 = Point_3(2, 0, 0)
p3 = Point_3(1, 1, 1)
p4 = Point_3(1, 0, 2)

mesh = Sm.make_tetrahedron(p1, p2, p3, p4)
fcp = {}
params = {}
num, fccmap = Pmp.connected_components(mesh)
print(num)
print(fccmap)
