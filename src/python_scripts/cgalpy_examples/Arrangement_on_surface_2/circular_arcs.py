#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY_kerEpecInt_aos2AlgPl'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Number_type = Ker.FT
Circle = Ker.Circle_2
Segment = Ker.Segment_2
Rational_point = Ker.Point_2
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Point = Arrangement.Point_2
Curve = Arrangement.Curve_2
CoordNT = Aos2.CoordNT

curves = []

# Create a circle (C1) centered at the origin with squared radius 2.
curves.append(Curve(Circle(Rational_point(0, 0), Number_type(2))))

# Create a circle (C2) centered at (2, 3) with radius 3/2. Note that
# as the radius is rational we use a different curve constructor.
three_halves = Number_type(3) / Number_type(2)
curves.append(Curve(Rational_point(2, 3), three_halves))

# Create a segment (C3) of the line (y = x) with rational endpoints.
s3 = Segment(Rational_point(-2, -2), Rational_point(2, 2))
curves.append(Curve(s3))

# Create a line segment (C4) with the same supporting line (y = x), but
# having one endpoint with irrational coordinates.
sqrt_15 = CoordNT(0, 1, 15) # = sqrt(15)
curves.append(Curve(s3.supporting_line(),
                    Point(3, 3), Point(sqrt_15, sqrt_15)))

# Create a circular arc (C5) that is the upper half of the circle centered at
# (1, 1) with squared radius 3. Create the circle with clockwise orientation,
# so the arc is directed from (1 - sqrt(3), 1) to (1 + sqrt(3), 1).
c5 = Rational_point (1, 1)
circ5 = Circle(c5, 3, Ker.CLOCKWISE)
one_minus_sqrt_3 = CoordNT(1, -1, 3)
one_plus_sqrt_3 = CoordNT(1, 1, 3)
s5 = Point(one_minus_sqrt_3, CoordNT(1))
t5 = Point(one_plus_sqrt_3, CoordNT(1))
curves.append(Curve(circ5, s5, t5))

# Create an arc (C6) of the unit circle, directed clockwise from
# (-1/2, sqrt(3)/2) to (1/2, sqrt(3)/2).
# The supporting circle is oriented accordingly.
c6 = Rational_point(0, 0)
half = Number_type(1) / Number_type(2)
sqrt_3_div_2 = CoordNT(Number_type(0), half, 3)
s6 = Point(-half, sqrt_3_div_2)
t6 = Point(half, sqrt_3_div_2)
curves.append(Curve(c6, 1, Ker.CLOCKWISE, s6, t6))

# Create a circular arc (C7) defined by two endpoints and a midpoint,
# all having rational coordinates. This arc is the upper right
# quarter of a circle centered at the origin with radius 5.
curves.append(Curve(Rational_point(0, 5), Rational_point(3, 4),
                    Rational_point(5, 0)))

# Construct the arrangement of the curves and print its size.
arr = Arrangement()
Aos2.insert(arr, curves)
print_arrangement_size(arr)
