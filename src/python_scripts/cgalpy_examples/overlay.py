#!/usr/bin/python3.9
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
Arr2 = CGALPY.Arr2

Point_2 = Ker.Point_2
Curve_2 = Arr2.Curve_2
Arrangement_2 = Arr2.Arrangement_2


arr1 = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(2, 0))
c2 = Curve_2(Point_2(2, 0), Point_2(2, 2))
c3 = Curve_2(Point_2(2, 2), Point_2(0, 2))
c4 = Curve_2(Point_2(0, 2), Point_2(0, 0))
Arr2.insert(arr1, [c1, c2, c3, c4])

arr2 = Arrangement_2()
c1 = Curve_2(Point_2(1, 1), Point_2(3, 1))
c2 = Curve_2(Point_2(3, 1), Point_2(3, 3))
c3 = Curve_2(Point_2(3, 3), Point_2(1, 3))
c4 = Curve_2(Point_2(1, 3), Point_2(1, 1))
Arr2.insert(arr2, [c1, c2, c3, c4])

# Set the data for the faces. The data can be any python object
for arr in [arr1, arr2]:
    for f in arr.faces():
        if f.is_unbounded():
            f.set_data(0)
        else:
            f.set_data(1)

    # Alternative
    # ubf: Face = arr.unbounded_face()
    # ubf.set_data(0)
    # f: Face
    # for ccb in ubf.inner_ccbs():
    #     next(next(ubf.inner_ccbs())).twin().face().set_data(1)

result = Arrangement_2()

# Set the data of a face that is the intersection of two faces
# from the original arrangements to be the sum of their data
overlay_traits = Arr2.Arr_face_overlay_traits(lambda x, y: x+y)
Arr2.overlay(arr1, arr2, result, overlay_traits)

for f in result.faces():
    print(f.data())

#  0  |----------|
#     |   1      |
# |----------|   |
# |   |  2   |   |
# |   |------|---|
# |  1       |
# |----------|
