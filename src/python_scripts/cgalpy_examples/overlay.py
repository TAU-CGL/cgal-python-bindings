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
Aos2 = CGALPY.Aos2

Arrangement_2 = Aos2.Arrangement_2
Point_2 = Arrangement_2.Geometry_traits_2.Point_2
Curve_2 = Arrangement_2.Geometry_traits_2.Curve_2

arr1 = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(2, 0))
c2 = Curve_2(Point_2(2, 0), Point_2(2, 2))
c3 = Curve_2(Point_2(2, 2), Point_2(0, 2))
c4 = Curve_2(Point_2(0, 2), Point_2(0, 0))
Aos2.insert(arr1, [c1, c2, c3, c4])

arr2 = Arrangement_2()
c1 = Curve_2(Point_2(1, 1), Point_2(3, 1))
c2 = Curve_2(Point_2(3, 1), Point_2(3, 3))
c3 = Curve_2(Point_2(3, 3), Point_2(1, 3))
c4 = Curve_2(Point_2(1, 3), Point_2(1, 1))
Aos2.insert(arr2, [c1, c2, c3, c4])

# Set the data for the faces. The data can be any python object
for arr in [arr1, arr2]:
  for f in arr.faces():
    f.set_data(0) if f.is_unbounded() else f.set_data(1)

  # Alternative
  # ubf: Face = arr.unbounded_face()
  # ubf.set_data(0)
  # f: Face
  # for ccb in ubf.inner_ccbs():
  #     next(next(ubf.inner_ccbs())).twin().face().set_data(1)

result = Arrangement_2()

# Set the data of a face that is the intersection of two faces
# from the original arrangements to be the sum of their data

# CGALPY specific:
# The Arr_face_overlay_traits object behaves much like the one in CGAL
# https://doc.cgal.org/latest/Arrangement_on_surface_2/classCGAL_1_1Arr__face__overlay__traits.html
# Instead of being templated with a functor, the functor is passed as a
# parameter to the constructor. Similarly the Arr_overlay_function traits
# object accepts the 10 functions to be implemented as described here:
# https://doc.cgal.org/latest/Arrangement_on_surface_2/classOverlayTraits.html
# As arguments to its constructor (in the same order as they are listed in the
# documentation). If you pass just one function, by default, it is the function
# that accepts the data items of two faces and computes the data item of the
# overlaid face.

# traits = Aos2.Arr_overlay_function_traits(lambda x, y: x+y)
# Aos2.overlay(arr1, arr2, result, traits)

# The Arr_overlay_traits is more generic. Here, you pass functions
# that accepts three cells. As in the case above, if you pass just one
# function, by default, it is the function that accepts the data items
# of two faces and computes the data item of the overlaid face.
traits = Aos2.Arr_overlay_traits(lambda f1, f2, f: f.set_data(f1.data()+f2.data()))
Aos2.overlay(arr1, arr2, result, traits)

for f in result.faces():
  print(f.data())

del traits
#  0  |----------|
#     |   1      |
# |----------|   |
# |   |  2   |   |
# |   |------|---|
# |  1       |
# |----------|
