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

# You can pass the polygon via an iterator pair
iss = Sn2.create_interior_straight_skeleton_2(points)

Sn2.print_straight_skeleton(iss)
Sn2.draw(iss, "Interior 2D Straight Skeleton")

# Or you can pass the polygon directly, as below.

# To create an exterior straight skeleton you need to specify a maximum offset.
max_offset = 5.0
oss = Sn2.create_exterior_straight_skeleton_2(max_offset, poly)

Sn2.print_straight_skeleton(oss);
Sn2.draw(oss, "Exterior 2D Straight Skeleton");
