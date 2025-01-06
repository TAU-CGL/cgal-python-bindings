#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Vis2 = CGALPY.Vis2
Point = Arrangement.Geometry_traits_2.Point_2
Segment = Arrangement.Geometry_traits_2.X_monotone_curve_2

p1 = Point(0,4)
p2 = Point(0,0)
p3 = Point(3,2)
p4 = Point(4,0)
p5 = Point(4,4)
p6 = Point(1,2)
segments = [Segment(p1, p2), Segment(p2, p3), Segment(p3, p4),
            Segment(p4, p5), Segment(p5, p6), Segment(p6, p1)]
env = Arrangement()
Aos2.insert_non_intersecting_curves(env, segments);

print(env.number_of_vertices(), ",", env.number_of_edges(), "," ,
      env.number_of_faces())

# find the face of the query point
# (usually you may know that by other means)
q = Point(0.5, 2);
Naive_pl = Aos2.Arr_naive_point_location
naive_pl = Naive_pl(env)
obj = naive_pl.locate(q)
# The query point locates in the interior of a face
assert(type(obj) == env.Face)
output = Arrangement()
Spv = Vis2.Simple_polygon_visibility_2
spv = Spv(env)
spv.compute_visibility(q, obj, output)
print("Non-regularized visibility region of q has", output.number_of_edges(),
      "edges:")
for e in output.edges():
    print("[", e.source().point(), "->", e.target().point(), "]")
