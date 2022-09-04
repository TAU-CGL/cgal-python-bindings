#!/usr/bin/python3
# export PYTHONPATH=...

import os
import sys
import importlib
import time
from construct_segment_arrangement import *
from point_location_utils import *
from read_objects import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2

# Get the name of the input file from the command line, or use the default
# fan_grids.dat file if no command-line parameters are given.
try:
  filename = argv[1]
except:
  filename = 'fan_grids.dat'

# Open the input file.
segments = read_objects(Aos2, filename)

# Construct the arrangement by aggregately inserting all segments.
arr = Arrangement()

print('Performing aggregated insertion of {} segments.'.format(len(segments)))
tic = time.perf_counter()
Aos2.insert(arr, segments)
toc = time.perf_counter()
print_arrangement_size(arr)
print(f"Construction took {toc - tic:0.4f} seconds")
