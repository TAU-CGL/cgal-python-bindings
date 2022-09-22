#!/usr/bin/python3
# export PYTHONPATH=...

import os
import sys
import importlib
from construct_segment_arrangement import *
from point_location_utils import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Segment = Arrangement.Geometry_traits_2.Curve_2
Point = Arrangement.Geometry_traits_2.Point_2

Naive_pl = Aos2.Arr_naive_point_location
Landmarks_pl = Aos2.Arr_landmarks_point_location

# Construct the arrangement.
arr = Arrangement()
construct_segment_arrangement(Aos2, arr)

# Perform some point-location queries using the naive strategy.
naive_pl = Naive_pl(arr)
locate_point(arr, naive_pl, Point(1, 4))          # q1
locate_point(arr, naive_pl, Point(4, 3))          # q2
locate_point(arr, naive_pl, Point(6, 3))          # q3

# Perform some point-location queries using the landmark strategy.
landmarks_pl = Landmarks_pl(arr)
locate_point(arr, landmarks_pl, Point(3, 2))      # q4
locate_point(arr, landmarks_pl, Point(5, 2))      # q5
locate_point(arr, landmarks_pl, Point(1, 0))      # q6
