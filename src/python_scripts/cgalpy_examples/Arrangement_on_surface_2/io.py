#!/usr/bin/python

import os
import sys
import importlib
from construct_segment_arrangement import *
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2

# Construct the arrangement.
arr1 = Arrangement()
construct_segment_arrangement(Aos2, arr1)
print("Writing")
print_arrangement_size(arr1)

# Write the arrangement to a file.
out_file = open("arr_ex_io.dat", 'w')
out_file.write(str(arr1))
out_file.close()

# Read the arrangement from the file.
in_file = open("arr_ex_io.dat", 'r')
arr2 = Arrangement(in_file.read())
in_file.close()

print("Reading")
print_arrangement_size(arr2)
