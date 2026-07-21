#!/usr/bin/python

import os
import sys
import importlib
from arr_print import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2
Segment = Arrangement.Geometry_traits_2.X_monotone_curve_2

S1 = [ Segment(Point(1, 3), Point(4, 6)),
       Segment(Point(1, 3), Point(6, 3)),
       Segment(Point(1, 3), Point(4, 0)),
       Segment(Point(4, 6), Point(6, 3)),
       Segment(Point(4, 0), Point(6, 3)) ]
s = Segment(Point(0, 3), Point(4, 3))
S2 = [ Segment(Point(0, 5), Point(6, 6)),
       Segment(Point(0, 4), Point(6, 5)),
       Segment(Point(0, 2), Point(6, 1)),
       Segment(Point(0, 1), Point(6, 0)),
       Segment(Point(6, 1), Point(6, 5)) ]

arr = Arrangement()
Aos2.insert_non_intersecting_curves(arr, S1)
Aos2.insert(arr, s);   	# 1 incremental
Aos2.insert(arr, S2)   	# 5 aggregate
print_arrangement_size(arr)
