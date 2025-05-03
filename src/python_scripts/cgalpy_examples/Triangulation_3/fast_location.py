#!/usr/bin/python

import os
import sys
import importlib
import random

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point = Ker.Point_3
Tri3 = CGALPY.Tri3

P = []
for z in range(20):
  for y in range(20):
    for x in range(20):
      P.append(Point(x,y,z));

# building their Delaunay triangulation.
T = Tri3.Delaunay_triangulation_3(P)
assert(T.number_of_vertices() == 8000)

# performing nearest vertex queries to a series of random points,
# which is a case where the Fast_location policy is beneficial.
for i in range(10000):
  q = Point(random.uniform(0, 20), random.uniform(0, 20), random.uniform(0, 20))
  v = T.nearest_vertex(q)
  print(f"{q}: {v.point()}")
