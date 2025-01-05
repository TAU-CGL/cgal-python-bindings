#!/usr/bin/python

import os
import sys
import importlib
from construct_segment_arrangement import *
from point_location_utils import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2

Walk_pl = Aos2.Arr_walk_along_line_point_location
Trap_pl = Aos2.Arr_trapezoid_ric_point_location

# Construct the arrangement.
arr = Arrangement()
construct_segment_arrangement(Aos2, arr)

# Perform some vertical ray-shooting queries using the walk strategy.
walk_pl = Walk_pl(arr)
shoot_vertical_ray(arr, walk_pl, Point(1, 4))
shoot_vertical_ray(arr, walk_pl, Point(4, 3))
shoot_vertical_ray(arr, walk_pl, Point(6, 3))

# Attach the trapezoid-RIC object to the arrangement and perform queries.
trap_pl = Trap_pl(arr)
shoot_vertical_ray(arr, trap_pl, Point(3, 2))
shoot_vertical_ray(arr, trap_pl, Point(5, 2))
shoot_vertical_ray(arr, trap_pl, Point(1, 0))
