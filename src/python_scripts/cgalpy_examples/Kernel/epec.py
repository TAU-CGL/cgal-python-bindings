#!/usr/bin/python3
# export PYTHONPATH=...

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
Point_2 = Ker.Point_2
Kernel = Ker.Kernel
# eq_2 =  Kernel.equal_2_object()
# cc_in_between_2 = Kernel.counterclockwise_in_between_2_object()

Counterclockwise_in_between_2 = Kernel.Counterclockwise_in_between_2
p = Point_2(1, -1,)
# p = Point_2(FT(1), FT(-1)) # error: FT is not defined for an inexact kernel
