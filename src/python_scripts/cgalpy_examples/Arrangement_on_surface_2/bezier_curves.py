#!/usr/bin/python3
# export PYTHONPATH=...

import os
import sys
import importlib
from arr_print import *
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
# Bezier.dat file if no command-line parameters are given.
try:
  filename = argv[1]
except:
  filename = 'Bezier.dat'

# Read the Bezier curves.
curves = read_objects(Aos2, filename)

arr = Arrangement()
# Construct the arrangement.
Aos2.insert(arr, curves)
print_arrangement_size(arr)
