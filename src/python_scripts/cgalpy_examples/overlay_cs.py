#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Ker = CGALPY.Ker
Point = Ker.Point_2
Circle = Ker.Circle_2
Arrangement = Aos2.Arrangement_2
Curve = Arrangement.Geometry_traits_2.Curve_2

# Construct the first arrangement
arr1 = Arrangement()
Aos2.insert(arr1,[Curve(Circle(Point(3,4),25)),Curve(Circle(Point(-3,-4),25))])

# Construct the second arrangement
arr2 = Arrangement()
Aos2.insert(arr2,[Curve(Circle(Point(4,-3),25)),Curve(Circle(Point(-4,3),25))])

# Set the data of the vertices. The data is an int python-object
for arr in [arr1, arr2]:
  for v in arr.vertices():
    v.set_data(v.degree())

# Compute the overlay and update the data of the vertices.
result = Arrangement()
traits = Aos2.Arr_overlay_function_traits()
traits.set_vv_v(lambda x, y: 2*x+y)
traits.set_ve_v(lambda x, y: 2*x+2)
traits.set_vf_v(lambda x, y: 2*x)
traits.set_ev_v(lambda x, y: 4+y)
traits.set_fv_v(lambda x, y: y)
traits.set_ee_v(lambda x, y: 6)
Aos2.overlay(arr1, arr2, result, traits)

for v in result.vertices(): print("({})".format(v.point()), v.data())

del traits
