#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import arr_print as ap

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY_kerEpec_aos2Alg'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
FT = Ker.FT
Aos2 = CGALPY.Aos2
Arrangement_2 = Aos2.Arrangement_2
arr = Arrangement_2()
Curve = Arrangement_2.Curve_2

# # Create a circle centered at the origin with radius 5 (C1).
Aos2.insert(arr, Curve(Ker.Circle_2(Ker.Point_2(0, 0), FT(25))));

# # Create a circle centered at (7,7) with radius 5 (C2).
Aos2.insert(arr, Curve(Ker.Circle_2(Ker.Point_2(7, 7), FT(25))));

# # Create a circle centered at (4,-0.5) with radius 3.5 (= 7/2) (C3).
c3 = Ker.Point_2(4, FT(-1) / FT(2))
Aos2.insert(arr, Curve(Ker.Circle_2(c3, FT(49) / FT(4))))

# Locate the vertex with maximal degree.
vertices = arr.vertices()
v_max = next(vertices)
print(v_max.point())
v: Arrangement_2.Vertex
for v in vertices:
  if (v.degree() > v_max.degree()): v_max = v

print('The vertex with maximal degree in the arrangement is: ' +
      'v_max = ({}) with degree {}.'.format(v_max.point(), v_max.degree()))
