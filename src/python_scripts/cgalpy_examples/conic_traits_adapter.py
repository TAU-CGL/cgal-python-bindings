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
Bso2 = CGALPY.Bso2
General_polygon_set = Bso2.General_polygon_set_2
Traits = General_polygon_set.Traits_2
Point = Traits.Point_2
Curve = Traits.Curve_2
X_monotone_curve = Traits.X_monotone_curve_2
Polygon = General_polygon_set.Polygon_2

def append_conic_arc(polygon, arc):
  traits = Traits()
  make_x_monotone = traits.make_x_monotone_2_object()
  objects = make_x_monotone(arc)
  for obj in objects:
    # if (CGAL::assign (xarc, *it))
    polygon.push_back(obj)

# Construct a parabolic arc supported by a parabola: x^2 + 2y - 4 = 0,
# and whose endpoints lie on the line y = 0:
parabola1 = Curve(1, 0, 0, 0, 2, -4, Ker.COUNTERCLOCKWISE,
                  Point(2, 0), Point(-2, 0))

# Construct a parabolic arc supported by a parabola: x^2 - 2y - 4 = 0,
# and whose endpoints lie on the line y = 0:
parabola2 = Curve(1, 0, 0, 0, -2, -4, Ker.COUNTERCLOCKWISE,
                  Point(-2, 0), Point(2, 0))

# Construct a polygon from these two parabolic arcs.
P = Polygon()
append_conic_arc(P, parabola1)
append_conic_arc(P, parabola2)

# Construct a polygon that corresponds to the ellipse: x^2 + 9y^2 - 9 = 0:
Q = Polygon()
append_conic_arc(Q, Curve(-1, -9, 0, 0, 0, 9))

# Compute the intersection of the two polygons.
print(type(P))

res = Bso2.intersection(P, Q)
print("The intersection:")
print(res)
