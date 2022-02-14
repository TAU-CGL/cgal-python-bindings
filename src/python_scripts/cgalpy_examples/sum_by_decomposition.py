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
MS2 = CGALPY.MS2
PP2 = CGALPY.PP2

def read_polygon(inp):
  pgn = Polygon()
  n = int(inp.readline())
  for i in range(n):
    line = inp.readline()
    lst = line.split()
    x = Ker.FT(float(lst[0]))
    y = Ker.FT(float(lst[1]))
    p = Point(x, y)
    pgn.push_back(p)
  return pgn

# Open the input file and read two polygons from it.
filename = sys.argv[2] if len(sys.argv) > 2 else "rooms_star.dat"
with open(filename, 'r') as inp:
  P = read_polygon(inp)
  print(P)
  Q = read_polygon(inp)
  print(Q)

  # Compute the Minkowski sum using the decomposition approach.
  # Minkowski sum by decompisition is temporarily commented out
  ssab_decomp = PP2.Small_side_angle_bisector_decomposition()
  sum1 = MS2.minkowski_sum_2(P, Q, ssab_decomp, ssab_decomp)
  sum2 = MS2.minkowski_sum_2(P, Q, ssab_decomp)
  sum3 = MS2.minkowski_sum_2(P, Q)
  print("P (+) Q = ", sum1)
  print("P (+) Q = ", sum2)
  print("P (+) Q = ", sum3)
