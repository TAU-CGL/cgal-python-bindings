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

# Construct the first arrangement, containing a square-shaped face.
arr1 = Arrangement()
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(2, 2), Point(6, 2)))
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(6, 2), Point(6, 6)))
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(6, 6), Point(2, 6)))
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(2, 6), Point(2, 2)))

# Construct the second arrangement, containing a rhombus-shaped face.
arr2 = Arrangement()
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(4, 1), Point(7, 4)))
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(7, 4), Point(4, 7)))
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(4, 7), Point(1, 4)))
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(1, 4), Point(4, 1)))

# Compute the overlay of the two arrangements.
overlay_arr = Arrangement()
Aos2.overlay(arr1, arr2, overlay_arr)
print_arrangement_size(overlay_arr)
