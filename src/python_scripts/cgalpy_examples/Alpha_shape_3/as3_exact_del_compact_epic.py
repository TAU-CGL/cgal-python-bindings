#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY_kerEpic_as3PlainExa_pol2_tri3DelAsAsComSeq'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
As3 = CGALPY.As3

Point_3 = Ker.Point_3
Alpha_shape_3 = As3.Alpha_shape_3
Mode = As3.Mode

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
for x in alphas: print(x)
