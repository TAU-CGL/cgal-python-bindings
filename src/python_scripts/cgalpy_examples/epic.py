#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_2 = Ker.Point_2
p = Point_2(1, -1,)
# p = Point_2(FT(1), FT(-1)) # error: FT is not defined for an inexact kernel
