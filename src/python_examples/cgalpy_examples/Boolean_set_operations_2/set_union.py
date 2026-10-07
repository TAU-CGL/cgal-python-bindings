#!/usr/bin/python3
# Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import importlib
import math
import sys

if len(sys.argv) < 2:
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Bso2 = CGALPY.Bso2

General_polygon_set = Bso2.General_polygon_set_2
Traits = General_polygon_set.Traits_2
Polygon = General_polygon_set.Polygon_2
Curve = Traits.Curve_2
Point = Ker.Point_2
Circle = Ker.Circle_2

def construct_polygon(circle):
  traits = Traits()
  make_x_monotone = traits.make_x_monotone_2_object()
  objects = make_x_monotone(Curve(circle))
  assert len(objects) == 2
  return Polygon([objects[0], objects[1]])

n_circles = 8
if len(sys.argv) > 2:
  n_circles = int(sys.argv[2])

radius = 1
frac = 2 * math.pi / n_circles
circles = []

for k in range(n_circles):
  angle = frac * k
  x = radius * math.sin(angle)
  y = radius * math.cos(angle)
  circles.append(construct_polygon(Circle(Point(x, y), radius)))

res = Bso2.join(circles, [])
for pwh in res:
  print(pwh)
print()
