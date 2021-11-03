#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import time

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

# Open the input file and read a polygon.
filename = sys.argv[2] if len(sys.argv) > 2 else "tight.dat"
with open(filename, 'r') as inp:
  P = read_polygon(inp)
  print("No. of vertices: ", P.size())

  # Approximate the offset polygon.
  inset_polygons = []
  tic = time.perf_counter()
  radius = Ker.FT(1)
  MS2.approximated_inset_2(P, radius, 0.00001, inset_polygons)
  toc = time.perf_counter()

  print("The inset comprises", len(inset_polygons), " polygon(s).")
  for pgn in inset_polygons:
    print(pgn)
  print(f"Inset computation took {toc - tic:0.4f} seconds")
