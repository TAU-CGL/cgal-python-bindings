#!/usr/bin/python3.9
# export PYTHONPATH=...

import os
import sys
import importlib

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY_kerEpic_as3Plain_pol2_tri3DelAsAsFasSeq'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
AS3 = CGALPY.AS3

Point_3 = Ker.Point_3
Alpha_shape_3 = AS3.Alpha_shape_3
Mode = AS3.Mode

p1 = Point_3(492, 291, 677)
p2 = Point_3(493, 314, 533)
p3 = Point_3(494, 326, 462)
p4 = Point_3(493, 303, 605)
alphaShape = Alpha_shape_3([p1, p2, p3, p4])

print("Alpha shape computed in REGULARIZED mode by default")
# Find alpha solid
alphaSolid = alphaShape.find_alpha_solid()
print("Smallest alpha value to get a solid through data points is ", alphaSolid)
# find optimal alpha value
optHandle = alphaShape.find_optimal_alpha(1)
optAlpha = next(optHandle)
print("Optimal alpha value to get one connected component is ", optAlpha)
alphaShape.set_alpha(optAlpha)
num = alphaShape.number_of_solid_components()
print("# solid components: ", num)
