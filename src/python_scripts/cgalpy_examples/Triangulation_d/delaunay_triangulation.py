#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Kerd = CGALPY.Kerd
Trid = CGALPY.Trid
Delaunay_triangulation = Trid.Delaunay_triangulation

pointsIn = [
    [ 42.89, 0, 60.55, 30.72, 0, 0, 0 ],
    [ 45.65, 50.83, 50.37, 16.13, 0, 0, 0 ],
    [ 79.06, 57.84, 61.59, 2.52, 0, 0, 0 ],
    [ 44.47, 39.46, 39.53, 28.72, 0, 0, 0 ],
    [ 0, 100, 0, 0, 100, 0, 53.47 ],
    [ 66.95, 100, 33.6, 0, 0, 0, 0 ],
    [ 42.89, 0, 0, 30.72, 100, 0, 53.47 ],
    [ 100, 100, 100, 100, 100, 100, 100 ]
]

epicd = Kerd.Kernel_d()
dt = Delaunay_triangulation(7, epicd)

points = []
Point_d = Kerd.Point_d
for idx, x in enumerate(pointsIn):
  p = Point_d(x)
  points.append(p)

i = 1
hint = dt.insert(points[0])
for p in points[1:]:
  hint = dt.insert(p, hint)
  print("Processing: {}/{}".format(i, len(points)))
  i += 1
print(dt.number_of_vertices(), dt.number_of_full_cells())
