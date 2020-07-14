#!/usr/bin/python3.7
# export PYTHONPATH=...

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

Point_3 = Ker.Point_3

p1 = Point_3(0, 0, 0);
p2 = Point_3(1, 0, 0);
p3 = Point_3(0, 1, 0);
p4 = Point_3(0, 0, 1);
p5 = Point_3(2, 2, 2);
p6 = Point_3(-1, 0, 1);

dt = Tri3.Delaunay_triangulation_3([p1, p2, p3, p4, p5, p6])
print(dt.is_valid(False, 0))
