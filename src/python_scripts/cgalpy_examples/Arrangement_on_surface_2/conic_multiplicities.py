#!/usr/bin/python
import os
import sys
import importlib
from enum import Enum
from arr_print import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Arrangement.Geometry_traits_2
Point = Traits.Point_2
Rational = Traits.Rational
Conic_arc = Traits.Curve_2
Naive_pl = Aos2.Arr_naive_point_location

Ker = CGALPY.Ker
Result = CGALPY.Result
Rat_point = Traits.Rat_point_2
Rat_segment = Traits.Rat_segment_2
Rat_circle = Traits.Rat_circle_2

traits = Traits()
ctr_cv = traits.construct_curve_2_object()

arr = Arrangement(traits)
pl = Naive_pl(arr);

# Insert a hyperbolic arc, supported by the hyperbola y = x^2/(1-x)
# (or: x^2 + xy - y = 0) with the endpoints (-1, 1/2) and (1/2, 1/2).
# Note that the arc is counterclockwise oriented.
ps1 = Point(-1, Rational(1,2));
pt1 = Point(Rational(1,2), Rational(1,2));
cv1 = ctr_cv(1, 0, 1, 0, -1, 0, Result.COUNTERCLOCKWISE, ps1, pt1);
Aos2.insert(arr, cv1, pl);

# Insert the bottom half of the circle centered at (0, 1/2) whose radius
# is 1/2 (therefore its squared radius is 1/4).
circ2 = Rat_circle(Rat_point(0, Rational(1,2)), Rational(1,4));
ps2 = Point(-Rational(1,2), Rational(1,2));
pt2 = Point(Rational(1,2), Rational(1,2));
cv2 = ctr_cv(circ2, Result.COUNTERCLOCKWISE, ps2, pt2);
Aos2.insert(arr, cv2, pl);

print_arrangement(arr);
