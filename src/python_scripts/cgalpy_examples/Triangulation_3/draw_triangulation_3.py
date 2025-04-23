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
Tri3 = CGALPY_TRI.Tri3
Point_3 = Ker.Point_3
Gog = CGALPY_GOG.Gog

generator = Gog.Random_points_in_sphere_3(1.0)
points = []
for i in range(50): points.append(next(generator))
dt3 = Tri3.Triangulation_3(points)
Tri3.draw(dt3, "3D Triangulation")
