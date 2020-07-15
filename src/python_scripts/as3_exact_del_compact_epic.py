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
Alpha_shape_3 = AS3.Alpha_shape_3
Mode = AS3.Mode

# Set of points for which the alpha shapes cannot be computed with
# a floating point alpha value (on certain platforms)
p1 = Point_3(49.2559, 29.1767, 6.7723)
p2 = Point_3(49.3696, 31.4775, 5.33777)
p3 = Point_3(49.4264, 32.6279, 4.6205)
p4 = Point_3(49.3127, 30.3271, 6.05503)
alphaShape = Alpha_shape_3([p1, p2, p3, p4], 0, Mode.GENERAL)
print("Alpha shape computed in GENERAL mode")
alphas = alphaShape.alphas()
# print(alphas)
for x in alphas:
  print(x)
