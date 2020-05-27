from arr2_epec_seg import *


arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(2, 0))
c2 = Curve_2(Point_2(2, 0), Point_2(2, 2))
c3 = Curve_2(Point_2(2, 2), Point_2(0, 2))
c4 = Curve_2(Point_2(0, 2), Point_2(0, 0))

insert(arr, [c1, c2, c3, c4])

insert_point(arr, Point_2(1, 1))

d = []

decompose(arr, d)
for pair in d:
    # pair is a tuple
    # pair[0] is an arrangement vertex
    # pair[1] is a pair holding the objects (vertex, halfedge, or face) above and below
    # the vertex(respectively), that is, the objects hit by the vertical walls emanating from the vertex
    v0 = pair[0]
    for obj in pair[1]:
        if obj.is_vertex():
            v1 = Vertex()
            obj.get_vertex(v1)
        elif obj.is_halfedge():
            he = Halfedge()
            obj.get_halfedge(he)
        elif obj.is_face():
            f = Face()
            obj.get_face(f)
