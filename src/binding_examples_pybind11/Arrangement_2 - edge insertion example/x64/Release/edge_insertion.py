from Arrangement_2 import *
arr = Arrangement_2()

s1 = Segment_2(Point_2(1, 3), Point_2(3, 5))
s2 = Segment_2(Point_2(3, 5), Point_2(5, 3))
s3 = Segment_2(Point_2(5, 3), Point_2(3, 1))
s4 = Segment_2(Point_2(3, 1), Point_2(1, 3))
s5 = Segment_2(Point_2(1, 3), Point_2(5, 3))

a = arr.unbounded_face()

e1 = arr.insert_in_face_interior(s1, arr.unbounded_face())
v1 = e1.val().source()
v2 = e1.val().target()
e2 = arr.insert_from_left_vertex(s2, v2, Face_handle())
v3 = e2.val().target()
e3 = arr.insert_from_right_vertex(s3, v3, Face_handle())
v4 = e3.val().target()
arr.insert_at_vertices(s4, v4, v1, Face_handle())
arr.insert_at_vertices(s5, v1, v3, Face_handle())
print_arrangement(arr)