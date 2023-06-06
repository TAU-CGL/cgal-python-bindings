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

Arrangement_2 = Aos2.Arrangement_2
Segment_2 = Ker.Segment_2
Curve_2 = Arrangement_2.Geometry_traits_2.Curve_2
Point_2 = Arrangement_2.Geometry_traits_2.Point_2

p0 = Point_2(0, 0)
p1 = Point_2(100, 100)
p2 = Point_2(0, 100)
p3 = Point_2(100, 0)
s0 = Segment_2(p0, p1)
c0 = Curve_2(s0)
c1 = Curve_2(Segment_2(p2, p3))
arr = Arrangement_2()
Aos2.insert(arr, c0)
Aos2.insert(arr, c1)

# Construct a point location data structure
naive_pl = Aos2.Arr_naive_point_location(arr)

# Perform some point-location queries using the naive strategy.
q1 = Point_2(1, 4)
q2 = Point_2(50, 50)

res_naive_1 = naive_pl.locate(q1)
assert(type(res_naive_1) == Aos2.Arrangement_2.Face)

res_naive_2 = naive_pl.locate(q2)
assert(type(res_naive_2) == Aos2.Arrangement_2.Vertex)
print(res_naive_2.point())

# Construct a different type of point location data structure
landmarks_pl = Aos2.Arr_landmarks_point_location(arr)
res_lm_1 = landmarks_pl.locate(q1)
assert(type(res_lm_1) == Aos2.Arrangement_2.Face)

res_lm_1.set_data("some data")
print(res_lm_1.data())

for face in arr.faces(): print(face.data())

# batch point location
batch_query = [q1, q2]
res = Aos2.locate(arr, batch_query)

del arr # the lifetime of the arrangement is tied to the lifetime of the results
assert(type(res[0][1]) == Aos2.Arrangement_2.Face)
assert(type(res[1][1]) == Aos2.Arrangement_2.Vertex)

print('Expect to get "other data":')
res[0][1].set_data("other data")

print(res[0][1].data())
print(res_naive_1.data())
print(res_lm_1.data())
