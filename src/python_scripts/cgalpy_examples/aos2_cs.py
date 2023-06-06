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
Ker_point = Ker.Point_2

Aos2 = CGALPY.Aos2
Arrangement_2 = Aos2.Arrangement_2
Geometry_traits_2 = Arrangement_2.Geometry_traits_2
Point = Geometry_traits_2.Point_2
X_monotone_curve = Geometry_traits_2.X_monotone_curve_2

gt = Geometry_traits_2()
cmp_y_at_x = gt.compare_y_at_x_2_object()
cmp_y_at_x_left = gt.compare_y_at_x_left_2_object()

p = Point(0, 0)
p0 = Ker_point(0, 0)
p1 = Ker_point(-1, 0)
p2 = Ker_point(1, 0)
c = X_monotone_curve(p1, p2)
res1 = cmp_y_at_x(p, c)
print(res1)

c1 = X_monotone_curve(Ker_point(-1, 1), p0)
c2 = X_monotone_curve(Ker_point(-1, -1), p0)
res2 = cmp_y_at_x_left(c1, c2, p)
print(res2)

arr = Arrangement_2(gt)
