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
assert(dt.dimension() == 3)

generator = Gog.Random_points_in_sphere_3()
for i in range(100):
  p = next(generator)
  lt, c = dt.locate_face(p)
  if lt == dt.Locate_type.VERTEX: continue # Point already exists
  bfs, cells = dt.find_conflicts(p, c)
  f = bfs[-1]
  if (len(cells) % 2) == 0:  #  Even number of conflict cells ?
    dt.insert_in_hole(p, cells, f.cell(), f.index)

print("Valid:", dt.is_valid())
print(f"Final triangulation has {dt.number_of_vertices()} vertices.")
Tri3.draw(dt, "3D Triangulation")
