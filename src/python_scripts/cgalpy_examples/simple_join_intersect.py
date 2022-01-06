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
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Polygon_with_holes = Pol2.Polygon_with_holes_2
BSO2 = CGALPY.BSO2

# Construct the two input polygons.
P = Polygon([Point(0, 0), Point(5, 0), Point(3.5, 1.5), Point(2.5, 0.5),
             Point(1.5, 1.5)])
print("P = ", P)

Q = Polygon([Point(0, 2), Point(1.5, 0.5), Point(2.5, 1.5), Point(3.5, 0.5),
             Point(5, 2)])
print("Q = ", Q)

# Compute the union of P and Q.
unionR = Polygon_with_holes()
if BSO2.join(P, Q, unionR):
  print("The union: ")
  print(unionR)
else:
  print("P and Q are disjoint and their union is trivial.")

# Compute the intersection of P and Q.
intR = BSO2.intersection(P, Q)

print("The intersection:")
print(intR)
