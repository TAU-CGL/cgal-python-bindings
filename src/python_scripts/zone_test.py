from arr2_epec_seg import *

arr = Arrangement_2()
p0 = Point_2(0, 0)
p1 = Point_2(1, 0)
p2 = Point_2(1, 1)
p3 = Point_2(0, 1)

insert(arr, Curve_2(Segment_2(p0, p1)))
insert(arr, Curve_2(Segment_2(p1, p2)))
insert(arr, Curve_2(Segment_2(p2, p3)))
insert(arr, Curve_2(Segment_2(p3, p0)))

res = []
c = Curve_2(Segment_2(Point_2(0, 0.5), Point_2(1, 1.5)))
zone(arr, c, res)
