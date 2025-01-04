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
Ker = CGALPY.Ker
p: Ker.Point_2
p = Ker.Point_2(1, 2)
print(p)

p = Ker.Point_2(CGALPY.Ker.Point_2(CGALPY.Ker.FT(1), CGALPY.Ker.FT(2)))
