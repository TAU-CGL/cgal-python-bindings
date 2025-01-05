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
Segment = Arrangement.Geometry_traits_2.Curve_2
Point = Arrangement.Geometry_traits_2.Point_2

# Construct the arrangement.
arr = Arrangement()
construct_segment_arrangement(Aos2, arr)

# Perform a batched point-location query.
points = [Point(1, 4), Point(4, 3), Point(6, 3), Point(3, 2), Point(5, 2), Point(1, 0)]
results = Aos2.locate(arr, points)

# Print the results.
for res in results: print_point_location(arr, res[0], res[1])

# Aos2.draw(arr)
