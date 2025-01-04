#!/usr/bin/python

import os
import sys
import importlib
from enum import Enum
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Result = CGALPY.Result
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Arrangement.Geometry_traits_2
Point = Traits.Point_2
Rational = Traits.Rational
Conic_arc = Traits.Curve_2

Ker = CGALPY.Ker
Rat_point = Traits.Rat_point_2
Rat_segment = Traits.Rat_segment_2
Rat_circle = Traits.Rat_circle_2

traits = Traits()
ctr_cv = traits.construct_curve_2_object()

arr = Arrangement(traits)

# Insert a hyperbolic arc (C1), supported by the hyperbola y = 1/x
# (or: xy - 1 = 0) with the endpoints (1/4, 4) and (2, 1/2).
# The arc is counterclockwise oriented.
p11 = Point(Rational(1,4), 4)
p12 = Point(2, Rational(1,2))
Aos2.insert(arr, ctr_cv(0, 0, 1, 0, 0, -1, Result.COUNTERCLOCKWISE, p11, p12))

# Insert a full ellipse (C2), which is (x/4)^2 + (y/2)^2 = 0 rotated by
# phi = 36.87 degrees (such that sin(phi) = 0.6, cos(phi) = 0.8),
# yielding: 58x^2 + 72y^2 - 48xy - 360 = 0.
Aos2.insert(arr, ctr_cv(58, 72, -48, 0, 0, -360))

# Insert the segment (C3) (1, 1) -- (0, -3).
Aos2.insert(arr, ctr_cv(Rat_segment(Rat_point(1, 1), Rat_point(0, -3))))

# Insert a circular arc (C4) supported by the circle x^2 + y^2 = 5^2,
# with (-3, 4) and (4, 3) as its endpoints. We want the arc to be
# clockwise-oriented, so it passes through (0, 5) as well.
c4 = ctr_cv(Rat_point(-3, 4), Rat_point(0, 5), Rat_point(4, 3))
Aos2.insert(arr, c4)

# Insert a full unit circle (C5) that is centered at (0, 4).
Aos2.insert(arr, ctr_cv(Rat_circle(Rat_point(0,4), 1)))

# Insert a parabolic arc (C6) supported by the parabola y = -x^2 with
# endpoints (-sqrt(3),-3) (~(-1.73,-3)) and (sqrt(2),-2) (~(1.41,-2)).
# Since the x-coordinates of the endpoints cannot be acccurately represented,
# we specify them as the intersections of the parabola with the lines
# y = -3 and y = -2, respectively. The arc is clockwise-oriented.
c6 = ctr_cv(1, 0, 0, 0, 1, 0, Result.CLOCKWISE,   # The parabola.
               Point(-1.73, -3),                  # approximation of the source.
               0, 0, 0, 0, 1, 3,                  # the line: y = -3.
               Point(1.41, -2),                   # approximation of the target.
               0, 0, 0, 0, 1, 2)                  # the line: y = -2.
Aos2.insert(arr, c6)

# Insert the right half of the circle centered at (4, 2.5) whose radius
# is 1/2 (therefore its squared radius is 1/4) (C7).
circ7 = Rat_circle(Rat_point(4, Rational(5,2)), Rational(1,4))
Aos2.insert(arr, ctr_cv(circ7, Result.CLOCKWISE, Point(4, 3), Point(4, 2)))

print_arrangement_size(arr)
