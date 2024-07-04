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
Rational_point = Ker.Point_2
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
arr = Arrangement()
Curve = Arrangement.Curve_2

# # Create a circle centered at the origin with radius 5 (C1).
Aos2.insert(arr, Curve(Circle(Rational_point(0, 0), Number_type(25))));

# # Create a circle centered at (7,7) with radius 5 (C2).
Aos2.insert(arr, Curve(Circle(Rational_point(7, 7), Number_type(25))));

# # Create a circle centered at (4,-0.5) with radius 3.5 (= 7/2) (C3).
c3 = Rational_point(4, Number_type(-1) / Number_type(2))
Aos2.insert(arr, Curve(Circle(c3, Number_type(49) / Number_type(4))))

# Locate the vertex with maximal degree.
vertices = arr.vertices()
v_max = next(vertices)
print(v_max.point())
v: Arrangement.Vertex
for v in vertices:
  if (v.degree() > v_max.degree()): v_max = v

print('The vertex with maximal degree in the arrangement is: ' +
      'v_max = ({}) with degree {}.'.format(v_max.point(), v_max.degree()))
