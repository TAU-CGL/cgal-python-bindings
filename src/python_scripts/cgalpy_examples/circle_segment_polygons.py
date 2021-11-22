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
BSO2 = CGALPY.BSO2
General_polygon_set = BSO2.General_polygon_set_2
Traits = General_polygon_set.Traits_2
Polygon = General_polygon_set.Polygon_2
X_monotone_curve = Traits.X_monotone_curve_2
curve = Traits.Curve_2
Point = Ker.Point_2
Circle = Ker.Circle_2

def ctr_circle_polygon(circle):
    # Subdivide the circle into two x-monotone arcs.
    cv = curve(circle)	# circle orientation is counterclockwise
    traits = Traits()
    make_x_monotone = traits.make_x_monotone_2_object()
    objects = []
    make_x_monotone(cv, objects);
    assert(len(objects) == 2)

    # Construct a polygon that comprises the two x-monotone arcs.
    p = Polygon()
    p.push_back(objects[0]);
    p.push_back(objects[1]);
    return p;

def ctr_quad_polygon(p1, p2, p3, p4):
    p = Polygon()
    p.push_back(X_monotone_curve(p1, p2))
    p.push_back(X_monotone_curve(p2, p3))
    p.push_back(X_monotone_curve(p3, p4))
    p.push_back(X_monotone_curve(p4, p1))
    return p

S = General_polygon_set()
S.insert(ctr_circle_polygon(Circle(Point(1, 1), 1)));
S.insert(ctr_circle_polygon(Circle(Point(5, 1), 1)));
S.insert(ctr_circle_polygon(Circle(Point(5, 5), 1)));
S.insert(ctr_circle_polygon(Circle(Point(1, 5), 1)));

S.join(ctr_quad_polygon(Point(1, 0), Point(5, 0), Point(5, 2), Point(1, 2)))
S.join(ctr_quad_polygon(Point(1, 4), Point(5, 4), Point(5, 6), Point(1, 6)));
S.join(ctr_quad_polygon(Point(0, 1), Point(2, 1), Point(2, 5), Point(0, 5)));
S.join(ctr_quad_polygon(Point(4, 1), Point(6, 1), Point(6, 5), Point(4, 5)));

assert(S.number_of_polygons_with_holes() == 1)
pwhs = []
S.polygons_with_holes(pwhs)
print(pwhs)

arr = S.arrangement()
print("# Cells: ", arr.number_of_vertices(), arr.number_of_edges(),
      arr.number_of_faces())
