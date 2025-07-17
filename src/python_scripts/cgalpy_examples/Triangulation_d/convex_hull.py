#!/usr/bin/python

import os
import sys
import importlib

lib_tri = 'CGALPY'
lib_gog = 'CGALPY'
lib_st = 'CGALPY'

if len(sys.argv) > 1:
  if sys.argv[1].startswith('CGALPY'):
    lib_tri = sys.argv[1]
    if len(sys.argv) > 2:
      if sys.argv[2].startswith('CGALPY'):
        lib_gog = sys.argv[2]
        if len(sys.argv) > 3:
          if sys.argv[3].startswith('CGALPY'):
            lib_st = sys.argv[3]

CGALPY_TRI = importlib.import_module(lib_tri)
CGALPY_GOG = importlib.import_module(lib_gog)
CGALPY_ST = importlib.import_module(lib_st)

Ker = CGALPY_TRI.Ker
Kerd = CGALPY_TRI.Kerd
Trid = CGALPY_TRI.Trid
Delaunay_triangulation = Trid.Delaunay_triangulation
Locate_type = Delaunay_triangulation.Locate_type
Gog = CGALPY_GOG.Gog
St = CGALPY_ST.St

points = []
generator = Gog.Random_points_in_cube_d(4)
for i in range(200): points.append(next(generator))

dt = Delaunay_triangulation()
St.spatial_sort(points, dt.geom_traits());

v = dt.insert(points[0])
hint = v.full_cell()
cnt = 1
for p in points[1:]:
  res = dt.locate_get_incident(p, hint)
  lt = res[0]
  # print("lt: ", lt)
  if lt == Locate_type.OUTSIDE_AFFINE_HULL:
    v = dt.insert(p)
    hint = v.full_cell()
    cnt += 1

  elif lt == Locate_type.OUTSIDE_CONVEX_HULL:
    fc = res[1]
    v = dt.insert(p, fc)
    hint = v.full_cell()
    cnt += 1

  else:
    hint = res[1]

print(F"{cnt} points were actually inserted.")
