#!/usr/bin/python

import os
import sys
import importlib
import time
from read_objects import *
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Aos2.Arr_non_caching_segment_traits_2
Segment = Traits.Curve_2

try:
  filename = argv[1]
except:
  filename = 'Europe.dat'

# Read the segments from the file.
# The input file format should be (all coordinate values are double
# precision floating-point numbers):
# <n>                                 # number of segments.
# <sx_1> <sy_1>  <tx_1> <ty_1>        # source and target of segment #1.
# <sx_2> <sy_2>  <tx_2> <ty_2>        # source and target of segment #2.
#   :      :       :      :
# <sx_n> <sy_n>  <tx_n> <ty_n>        # source and target of segment #n.
segments = read_objects(Segment, filename)

# Construct the arrangement by aggregately inserting all segments.
arr = Arrangement()

print('Performing aggregated insertion of {} segments.'.format(len(segments)))
tic = time.perf_counter()
Aos2.insert_non_intersecting_curves(arr, segments)
toc = time.perf_counter()
print_arrangement_size(arr)
print(f"Construction took {toc - tic:0.4f} seconds")
