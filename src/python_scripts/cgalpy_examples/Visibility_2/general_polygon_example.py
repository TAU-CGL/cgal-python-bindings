#!/usr/bin/python

import importlib
import sys

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Vis2 = CGALPY.Vis2

Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2
Segment = Arrangement.Geometry_traits_2.X_monotone_curve_2

p1 = Point(1, 2)
p2 = Point(12, 3)
p3 = Point(19, -2)
p4 = Point(12, 6)
p5 = Point(14, 14)
p6 = Point(9, 5)

h1 = Point(8, 3)
h2 = Point(10, 3)
h3 = Point(8, 4)
h4 = Point(10, 6)
h5 = Point(11, 6)
h6 = Point(11, 7)

segments = [
    Segment(p1, p2),
    Segment(p2, p3),
    Segment(p3, p4),
    Segment(p4, p5),
    Segment(p5, p6),
    Segment(p6, p1),
    Segment(h1, h2),
    Segment(h2, h3),
    Segment(h3, h1),
    Segment(h4, h5),
    Segment(h5, h6),
    Segment(h6, h4),
]

env = Arrangement()
Aos2.insert_non_intersecting_curves(env, segments)

query_point = p4
halfedge = None
for candidate in env.halfedges():
    if candidate.source().point() == p3 and candidate.target().point() == p4:
        halfedge = candidate
        break

assert halfedge is not None

output_arr = Arrangement()
tev = Vis2.Triangular_expansion_visibility_2(env)
face = tev.compute_visibility(query_point, halfedge, output_arr)

print(f"Regularized visibility region of q has {output_arr.number_of_edges()} edges.")
print("Boundary edges of the visibility region:")

outer_ccb = face.outer_ccb()
first_edge = next(outer_ccb)
print(f"[{first_edge.source().point()} -> {first_edge.target().point()}]")

for edge in outer_ccb:
    print(f"[{edge.source().point()} -> {edge.target().point()}]")
