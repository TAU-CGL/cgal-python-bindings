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

Point_2 = KER.Point_2
Segment_2 = KER.Segment_2

s1 = Segment_2(Point_2(0, 1), Point_2(1,0))
s2 = Segment_2(Point_2(0, 0), Point_2(1,1))
ix = KER.intersection(s1, s2)
if not ix.is_point():
  sys.exit('Wrong intersection')
p = Point_2()
ix.get_point(p)
print(p)
