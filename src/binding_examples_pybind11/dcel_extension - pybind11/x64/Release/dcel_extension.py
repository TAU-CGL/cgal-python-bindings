from Arrangement_2 import *
arr = Arrangement_2()

s1 = Segment_2(Point_2(4, 1), Point_2(7, 6))
s2 = Segment_2(Point_2(1, 6), Point_2(7, 6))
s3 = Segment_2(Point_2(4, 1), Point_2(1, 6))
s4 = Segment_2(Point_2(1, 3), Point_2(7, 3))
s5 = Segment_2(Point_2(1, 3), Point_2(4, 8))
s6 = Segment_2(Point_2(4, 8), Point_2(7, 3))

insert_non_intersecting_curve (arr, s1)
insert_non_intersecting_curve (arr, s2)
insert_non_intersecting_curve (arr, s3)
insert (arr, s4)
insert (arr, s5)
insert (arr, s6)

vit = arr.vertices_begin()

while vit != arr.vertices_end():
    v = vit.val()
    if v.degree() == 0:
        v.set_data(BLUE)
    elif v.degree() <= 2:
        v.set_data(RED)
    else:
        v.set_data(WHITE)
    vit.inc()

fit = arr.faces_begin()
while fit != arr.faces_end():
    boundary_size = 0
    f = fit.val()
    if not f.is_unbounded():
        curr = f.outer_ccb()
        flag = True
        while(flag):
            boundary_size += 1
            curr.inc()
            flag = (curr != f.outer_ccb())
    f.set_data(boundary_size)
    fit.inc()

print_arr(arr)

fit = arr.faces_begin()
i = 0
while fit != arr.faces_end():
    f = fit.val()
    f.set_data(i)
    i+=1
    fit.inc()

f = arr.faces_begin().val()
faceVals = [0 for j in range(i)]
faceVals[0] = "some value"

print(faceVals[f.data()])