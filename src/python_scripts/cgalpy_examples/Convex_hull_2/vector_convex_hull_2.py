#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Ch2 = CGALPY.Ch2
Point_2 = Ker.Point_2

points = [Point_2(0,0), Point_2(10,0), Point_2(10,10), Point_2(6,5), Point_2(4,1)]
print(Ch2.ch_akl_toussaint(points))
print(Ch2.ch_bykat(points))
print(Ch2.ch_eddy(points))
print(Ch2.ch_graham_andrew(points))
print(Ch2.ch_jarvis(points))
print(Ch2.ch_melkman(points))
print(Ch2.convex_hull_2(points))
