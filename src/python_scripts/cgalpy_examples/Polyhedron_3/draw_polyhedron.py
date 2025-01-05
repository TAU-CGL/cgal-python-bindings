#!/usr/bin/python

import os
import sys
import importlib
import os.path

lib = 'CGALPY'
filename = 'meshes/cross_quad.off'
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    if len(sys.argv) > 2: filename = sys.argv[2]
  else:
    filename = str
    if len(sys.argv) > 2: lib = sys.argv[2]

CGALPY = importlib.import_module(lib)
fullname = CGALPY.data_file_path(filename)
if not os.path.isfile(fullname): fullname = filename

Pol3 = CGALPY.Pol3
Polyhedron = Pol3.Polyhedron_3

with open(fullname, 'r') as f:
  prn = Polyhedron(f.read())
  Pol3.draw(prn, filename)
