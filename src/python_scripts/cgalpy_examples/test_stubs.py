#!/usr/bin/python3
import os
import sys
import importlib

# if len(sys.argv) < 2:
#   sys.path.append(os.path.abspath('../precompiled'))
#   lib = 'CGALPY'
# else:
#   lib = sys.argv[1]

# CGALPY = importlib.import_module(lib)

import CGALPY_kerEpecInt_aos2SegPl as CGALPY
Ker = CGALPY.Ker
p: Ker.Point_2
p = Ker.Point_2(1, 2)
print(p)

# this fails for me:
p = Ker.Point_2(CGALPY.Kernel.Point_2(CGALPY.Ker.FT(1), CGALPY.Ker.FT(2)))
