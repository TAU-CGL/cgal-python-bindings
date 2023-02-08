#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import time
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
Ms2 = CGALPY.Ms2
PP2 = CGALPY.Pp2

def read_polygon(inp):
  pgn = Polygon()
  n = int(inp.readline())
  for i in range(n):
    line = inp.readline()
    lst = line.split()
    x = Ker.FT(lst[0])
    y = Ker.FT(lst[1])
    p = Point(x, y)
    pgn.push_back(p)
  return pgn

filename = sys.argv[2] if len(sys.argv) > 2 else "spiked.dat"
with open(filename, 'r') as inp:
  P = read_polygon(inp)
  tic = time.perf_counter()
  offset = Ms2.approximated_offset_2(P, 5, 0.00001);
  toc = time.perf_counter()
  print("The offset polygon has", offset.outer_boundary().size(),
        " vertices, ", offset.number_of_holes(), "holes.")
  print(f"Construction took {toc - tic:0.4f} seconds")
