#!/usr/bin/python
import os
import sys
import importlib
if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

tm1 = Sm.make_tetrahedron(Point(0,0,0), Point(2,0,0), Point(1,1,1), Point(1,0,2))

face_list = list(tm1.faces())
tm2 = Pmp.isotropic_remeshing(face_list, 0.05, tm1)

dist = Pmp.approximate_Hausdorff_distance(tm1, tm2, {'number_of_points_per_area_unit': 4000})

print(f"Approximated Hausdorff distance: {round(dist, 7)}")
