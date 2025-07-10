#!/usr/bin/python

import os
import sys
import importlib

lib_tri = 'CGALPY'
lib_gog = 'CGALPY'

if len(sys.argv) > 1:
  if sys.argv[1].startswith('CGALPY'):
    lib_tri = sys.argv[1]
    if len(sys.argv) > 2:
      if sys.argv[2].startswith('CGALPY'):
        lib_gog = sys.argv[2]

CGALPY_TRI = importlib.import_module(lib_tri)
CGALPY_GOG = importlib.import_module(lib_gog)

Ker = CGALPY_TRI.Ker
Tri2 = CGALPY_TRI.Tri2
Point_2 = Ker.Point_2
Gog = CGALPY_GOG.Gog

generator = Gog.Random_points_in_disc_2(1.0)
points = []
for i in range(10): points.append(next(generator))
dt = Tri2.Delaunay_triangulation_2(points)
Tri2.draw(dt, "2D Delaunay Triangulation")
