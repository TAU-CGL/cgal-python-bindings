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
Ker = CGALPY.ker
Point = Ker.Point_2
Pol2 = CGALPY.pol2
Polygon = Pol2.Polygon_2
Polygon_with_holes = Pol2.Polygon_with_holes_2
BSO2 = CGALPY.bso2
Polygon_set = BSO2.Polygon_set_2
# Traits = Polygon_set.Traits_2
# print(type(Traits))

# Construct the two initial polygons and the clipping rectangle.
P = Polygon()
P.push_back(Point(0, 1))
P.push_back(Point(2, 0))
P.push_back(Point(1, 1))
P.push_back(Point(2, 2))

Q = Polygon()
Q.push_back(Point(3, 1))
Q.push_back(Point(1, 2))
Q.push_back(Point(2, 1))
Q.push_back(Point(1, 0))

rect = Polygon()
rect.push_back(Point(0, 0))
rect.push_back(Point(3, 0))
rect.push_back(Point(3, 2))
rect.push_back(Point(0, 2))

# Perform a sequence of operations.
S = Polygon_set()
S.insert(P)
S.join(Q)                   # Compute the union of P and Q.
S.complement()               # Compute the complement.
S.intersection(rect)        # Intersect with the clipping rectangle.

# Print the result.
print("The result contains ", S.number_of_polygons_with_holes(), " components:")
res = S.polygons_with_holes()
print(res)
