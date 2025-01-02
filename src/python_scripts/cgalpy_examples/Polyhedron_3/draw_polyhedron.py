#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Polyhedron = Pol3.Polyhedron_3

# Get the name of the input file from the command line, or use the default
# file if no command-line parameters are given.
try:
  filename = argv[1]
except:
  filename = 'meshes/cross_quad.off'

with open(filename, 'r') as f:
  prn = Polyhedron(f.read())
  Pol3.draw(prn, filename)
