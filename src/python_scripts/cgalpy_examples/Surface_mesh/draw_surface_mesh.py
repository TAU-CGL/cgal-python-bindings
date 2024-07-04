#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]

CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm

# Get the name of the input file from the command line, or use the default
# file if no command-line parameters are given.
filename = sys.argv[1] if len(sys.argv) > 1 else 'meshes/elephant.off'

sm = Sm.read_polygon_mesh(filename)
Sm.draw(sm)
