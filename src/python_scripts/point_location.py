from arr2_epec_seg import *

p0 = Point_2(0, 0)
p1 = Point_2(100, 100)
p2 = Point_2(0, 100)
p3 = Point_2(100, 0)
s0 = Segment_2(p0, p1)
c0 = Curve_2(s0)
c1 = Curve_2(Segment_2(p2, p3))
arr = Arrangement_2()
insert(arr, c0)
insert(arr, c1)

# Construct a point location data structure
naive_pl = Arr_naive_point_location(arr)

# Perform some point-location queries using the naive strategy.
q1 = Point_2(1, 4)
res = naive_pl.locate(q1)
assert(type(res) == Arr_point_location_result)
assert(res.is_face())
q2 = Point_2(50, 50)
res = naive_pl.locate(q2)
assert(res.is_vertex())
if res.is_vertex():
    v = Vertex()
    res.get_vertex(v)
    print(v.point())

# Construct a different type of point location data structure
landmarks_pl = Arr_landmarks_point_location(arr)
q1 = Point_2(1, 4)
res = landmarks_pl.locate(q1)
