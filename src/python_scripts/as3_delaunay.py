#!/usr/bin/python3.7

# export PYTHONPATH="/home/efif/build/cgalpy/delaunay_release/src/libs/tri3"

from tri3_epic import *
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
