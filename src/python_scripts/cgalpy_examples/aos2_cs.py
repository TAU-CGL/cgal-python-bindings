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
Aos2 = CGALPY.Aos2
Arrangement_2 = Aos2.Arrangement_2
Point_2 = Arrangement_2.Geometry_traits_2.Point_2
Curve_2 = Arrangement_2.Geometry_traits_2.Curve_2

arr = Arrangement_2()
