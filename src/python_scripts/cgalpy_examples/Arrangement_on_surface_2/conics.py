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
Aos2 = CGALPY.Aos2
Arr =  Aos2.Arrangement_2
Traits = Aos2.Geometry_traits_2
Curve = Traits.Curve_2
X_monotone_curve = Traits.X_monotone_curve_2
Point = Traits.Point_2
Rational = Traits.Rational

Rat_point = Traits.Rat_point_2
Rat_segment = Traits.Rat_segment_2
Rat_circle = Traits.Rat_circle_2

arr = Arr()

# Insert a hyperbolic arc (C1), supported by the hyperbola y = 1/x
# (or: xy - 1 = 0) with the endpoints (1/4, 4) and (2, 1/2).
# The arc is counterclockwise oriented.
p1 = Point(Rational(1,4), 4)
p2 = Point(2, Rational(1,2))
cv1 = Curve(0, 0, 1, 0, 0, -1, Ker.COUNTERCLOCKWISE, p1, p2)
Aos2.insert(arr, cv1)

# Insert a full ellipse (C2), which is (x/4)^2 + (y/2)^2 = 0 rotated by
# phi = 36.87 degrees (such that sin(phi) = 0.6, cos(phi) = 0.8),
# yielding: 58x^2 + 72y^2 - 48xy - 360 = 0.
cv2 = Curve(58, 72, -48, 0, 0, -360)
Aos2.insert(arr, cv2)

# Insert the segment (C3) (1, 1) -- (0, -3).
seg3 = Rat_segment(Rat_point(1, 1), Rat_point(0, -3))
cv3 = Curve(seg3)
Aos2.insert(arr, cv3)

# Insert a circular arc (C4) supported by the circle x^2 + y^2 = 5^2,
# with (-3, 4) and (4, 3) as its endpoints. We want the arc to be
# clockwise-oriented, so it passes through (0, 5) as well.
cv4 = Curve(Rat_point(-3, 4), Rat_point(0, 5), Rat_point(4, 3))
Aos2.insert(arr, cv4)

# Insert a full unit circle (C5) that is centered at (0, 4).
circle5 = Rat_circle(Rat_point(0,4), 1)
cv5 = Curve(circle5)
Aos2.insert(arr, cv5)

# Insert a parabolic arc (C6) supported by the parabola y = -x^2 with
# endpoints (-sqrt(3),-3) (~(-1.73,-3)) and (sqrt(2),-2) (~(1.41,-2)).
# Since the x-coordinates of the endpoints cannot be acccurately represented,
# we specify them as the intersections of the parabola with the lines
# y = -3 and y = -2, respectively. The arc is clockwise-oriented.
cv6 = Curve(1, 0, 0, 0, 1, 0, Ker.CLOCKWISE,	# The parabola.
            Point(-1.73, -3),        		# approximation of the source.
            0, 0, 0, 0, 1, 3,        		# the line: y = -3.
            Point(1.41, -2),         		# approximation of the target.
            0, 0, 0, 0, 1, 2)        		# the line: y = -2.
Aos2.insert(arr, cv6)

# Insert the right half of the circle centered at (4, 2.5) whose radius
# is 1/2 (therefore its squared radius is 1/4) (C7).
circ7 = Rat_circle(Rat_point(4, Rational(5,2)), Rational(1,4))
cv7 = Curve(circ7, Ker.CLOCKWISE, Point(4, 3), Point(4, 2))
Aos2.insert(arr, cv7)

print(arr.number_of_vertices(), arr.number_of_edges(), arr.number_of_faces())
