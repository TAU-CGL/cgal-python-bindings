#!/usr/bin/python3.7
# export PYTHONPATH=/home/efif/build/cgalpy/release/{fixed/}arr2/src/libs/cgalpy

import sys
import importlib

if len(sys.argv) < 2:
  sys.exit('Library name missing')

mdl = importlib.import_module(sys.argv[1])

# is there an __all__?  if so respect it
if "__all__" in mdl.__dict__:
  names = mdl.__dict__["__all__"]
else:
  # otherwise we import all names that don't begin with _
  names = [x for x in mdl.__dict__ if not x.startswith("_")]

# now drag them in
globals().update({k: getattr(mdl, k) for k in names})

arr = Arrangement_2()
c1 = Curve_2(Point_2(0, 0), Point_2(2, 0))
c2 = Curve_2(Point_2(1, 2), Point_2(1, -2))
c3 = Curve_2(Point_2(0, 0), Point_2(1, 2))
c4 = Curve_2(Point_2(1, -2), Point_2(2, 0))
insert(arr, [c1, c2, c3, c4])
print("Number of faces in the arrangement:", arr.number_of_faces())
print("Number of halfedges in the arrangement:", arr.number_of_halfedges())
print("Number of vertices in the arrangement:", arr.number_of_vertices())

# Iteration example
v: Vertex
for v in arr.vertices(): print(v.point())

e: Halfedge
for e in arr.halfedges(): print(e.curve())

f: Face
for f in arr.faces(): print(f.is_unbounded())
