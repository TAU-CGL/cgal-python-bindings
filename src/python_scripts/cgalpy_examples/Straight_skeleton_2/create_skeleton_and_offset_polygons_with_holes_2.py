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

points = [Point(0.0, 0.0), Point(10.0, 0.0), Point(10.0, 4.5), Point(12.0, 4.5), Point(12.0, 2.0), Point(16.0, 2.0),
          Point(16.0, 8.0), Point(12.0, 8.0), Point(12.0, 5.5), Point(10.0, 5.5), Point(10.0, 10.0), Point(0.0, 10.0)]
outer = Polygon(points)
assert(outer.is_counterclockwise_oriented())

hole_points = [Point(3.0,3.0), Point(3.0,7.0), Point(7.0,7.0), Point(7.0,3.0)]
hole = Polygon(hole_points)
assert(hole.is_clockwise_oriented())

poly = Polygon_with_holes(outer)
poly.add_hole(hole)

offset = 1.0
offset_pwhs = Sn2.create_interior_skeleton_and_offset_polygons_with_holes_2(offset, poly)
Sn2.print_polygons_with_holes(offset_pwhs)
