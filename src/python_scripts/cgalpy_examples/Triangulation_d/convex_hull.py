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
Kerd = CGALPY_TRI.Kerd
Trid = CGALPY_TRI.Trid
Delaunay_triangulation = Trid.Delaunay_triangulation
Gog = CGALPY_GOG.Gog

points = []
generator = Gog.Random_points_in_cube_d(4)
for i in range(20): points.append(next(generator))

# spatial_sort(points, epicd);

dt = Delaunay_triangulation()
res = dt.locate(points[0])
print(res)
# lt = res[1][0]
# assert(lt != Trid.OUTSIDE_CONVEX_HULL and lt != Trid.OUTSIDE_AFFINE_HULL)
# f = res[1][1]
# ft = res[1][2]
# fch = res[1][3]
# v = dt.insert(p, lt, f, ft, fch)
# hint = v.full_cell()

cnt = 1
# for p in points[1:]:
#   res = dt.locate(p, hint)
#   lt = res[1][0]
#   if lt != Trid.OUTSIDE_CONVEX_HULL and lt != Trid.OUTSIDE_AFFINE_HULL:
#     hint = fch
#     continue
#   f = res[1][1]
#   ft = res[1][2]
#   fch = res[1][3]
#   v = dt.insert(p, lt, f, ft, fch)
#   hint = v.full_cell()
#   cnt += 1

print(F"{cnt} points were actually inserted.")
