#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]
CGALPY = importlib.import_module(lib)

Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Sn2 = CGALPY.Sn2

points = [Point(-1,-1), Point(0,-12), Point(1,-1), Point(12,0), Point(1,1), Point(0,12), Point(-1,1), Point(-12,0)]
poly = Polygon(points)
assert(poly.is_counterclockwise_oriented())
ss = Sn2.create_interior_straight_skeleton_2(poly)

offset = 1.0
offset_polygons = Sn2.create_offset_polygons_2(offset, ss)
Sn2.print_polygons(offset_polygons)
