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
Polygon_with_holes = Pol2.Polygon_with_holes_2
Sn2 = CGALPY.Sn2

points = [Point(-1,-1), Point(0,-12), Point(1,-1), Point(12,0), Point(1,1), Point(0,12), Point(-1,1), Point(-12,0)]
outer = Polygon(points)
assert(outer.is_counterclockwise_oriented())

hole_points = [Point(-1,0), Point(0,1), Point(1,0), Point(0,-1)]
hole = Polygon(hole_points)
assert(hole.is_clockwise_oriented())

poly = Polygon_with_holes(outer)
poly.add_hole(hole)

iss = Sn2.create_interior_straight_skeleton_2(poly)
Sn2.print_straight_skeleton(iss);
