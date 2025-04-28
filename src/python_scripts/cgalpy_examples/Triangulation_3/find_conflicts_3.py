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
Point = Ker.Point_3
Gog = CGALPY_GOG.Gog

dt = Tri3.Triangulation_3()
dt.insert(Point(0,0,0))
dt.insert(Point(1,0,0))
dt.insert(Point(0,1,0))
dt.insert(Point(0,0,1))
Tri3.draw(dt, "3D Triangulation")

generator = Gog.Random_points_in_sphere_3()
for i in range(100):
  p = next(generator)
  res = dt.locate_face(p)
  if isinstance(res, tuple):
    if res[0] == Tri3.Locate_type.VERTEX: continue # Point already exists
#   vec = find_conflicts(p, c, CGAL::Oneset_iterator<Facet>(f))
#   if (len(vec) & 1) == 0:  #  Even number of conflict cells ?
#     dt.insert_in_hole(p, vec, f.first, f.second)

print(f"Final triangulation has {dt.number_of_vertices()} vertices.")
