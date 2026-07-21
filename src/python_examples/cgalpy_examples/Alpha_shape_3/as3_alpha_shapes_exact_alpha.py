#!/usr/bin/python

# Author: Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import importlib
import sys

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
As3 = CGALPY.As3

Point_3 = Ker.Point_3
Alpha_shape_3 = As3.Alpha_shape_3
Mode = Alpha_shape_3.Mode

# Same point set as CGAL Alpha_shapes_3/ex_alpha_shapes_exact_alpha.cpp.
points = [
    Point_3(49.2559, 29.1767, 6.7723),
    Point_3(49.3696, 31.4775, 5.33777),
    Point_3(49.4264, 32.6279, 4.6205),
    Point_3(49.3127, 30.3271, 6.05503),
]

# The C++ example has no stdout; this script intentionally prints nothing.
Alpha_shape_3(points, 0, Mode.GENERAL)
