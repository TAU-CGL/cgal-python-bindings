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

p1 = Point_3(1, 0, 0)
p2 = Point_3(0, 1, 0)
p3 = Point_3(0, 0, 1)
alphaShape  = Alpha_shape_3([p1, p2, p3])
# alphaShape.make_alpha_shape([p1, p2, p3])

print("Alpha shape computed in REGULARIZED mode by default")
# find optimal alpha value
optHandle = alphaShape.find_optimal_alpha(1);
print(optHandle)
# optIter = optHandle.__iter__();
optAlpha = next(optHandle)
print("Optimal alpha value to get one connected component is ", optAlpha)
alphaShape.set_alpha(optAlpha);
num = alphaShape.number_of_solid_components()
print("# solid components: ", num)
