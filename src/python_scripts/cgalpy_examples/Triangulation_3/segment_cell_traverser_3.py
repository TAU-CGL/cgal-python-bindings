#!/usr/bin/python

import os
import sys
import importlib

lib_tri3 = 'CGALPY'
lib_psp = 'CGALPY'
filename = ''

if len(sys.argv) > 1:
  if sys.argv[1].startswith('CGALPY'):
    lib_tri3 = sys.argv[1]
    if len(sys.argv) > 2:
      if sys.argv[2].startswith('CGALPY'):
        lib_psp = sys.argv[2]
        if len(sys.argv) > 3: filename = sys.argv[3]
      else: filename = sys.argv[2]
  else: filename = sys.argv[1]

CGALPY_TRI3 = importlib.import_module(lib_tri3)
CGALPY_PSP = importlib.import_module(lib_psp)

if not filename: filename = CGALPY_PSP.data_file_path("points_3/blobby.xyz")
points = CGALPY_PSP.Psp.read_points(filename)
Tri3 = CGALPY_TRI3.Tri3
dt = Tri3.Triangulation_3(points)
assert(dt.is_valid())
Tri3.draw(dt, "3D Delaunay")

xmin = points[0].x()
xmax = points[0].x()
ymin = points[0].y()
ymax = points[0].y()
zmin = points[0].z()
zmax = points[0].z()

for p in points:
  xmin = min(xmin, p.x())
  ymin = min(ymin, p.y())
  zmin = min(zmin, p.z())
  xmax = max(xmax, p.x())
  ymax = max(ymax, p.y())
  zmax = max(zmax, p.z())

Ker = CGALPY_TRI3.Ker
Point_3 = Ker.Point_3

rnd = CGALPY_TRI3.Random()
p1 = Point_3(rnd.get_double(xmin, xmax),
             rnd.get_double(ymin, ymax),
             rnd.get_double(zmin, zmax))
p2 = Point_3(rnd.get_double(xmin, xmax),
             rnd.get_double(ymin, ymax),
             rnd.get_double(zmin, zmax))

# Count the number of finite cells traversed.
inf = int(0)
fin_ = int(0)
for c in dt.segment_traverser_cells(p1, p2):
  if dt.is_infinite(c): inf += 1
  else: fin_ += 1

print(f"While traversing from {p1} to {p2}")
print(f"{inf} infinite and {fin_} finite cells were visited.")
print()

# Construct a traverser and use range-iterator

# Count the number of finite cells traversed.
inf = 0
fin_ = 0
for c in dt.segment_traverser_cells(p1, p2):
  if dt.is_infinite(c): inf += 1;
  else: fin_ += 1;

print(f"While traversing from {p1}  to {p2}")
print(f"{inf}  infinite and {fin_} finite cells were visited.")
