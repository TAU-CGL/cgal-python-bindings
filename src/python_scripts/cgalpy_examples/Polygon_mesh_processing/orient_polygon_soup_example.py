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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = "meshes/tet-shuffled.off" if len(sys.argv) < 3 else sys.argv[2]

try:
    points, polygons = Sm.read_polygon_soup(filename)
except:
    print("Cannot open file ")
    exit(1)

if len(points) == 0:
    print("Cannot open file ")
    exit(1)

def dv(a):
    print(a)

def g(a, b):
    print(a, b)

v = Pmp.Default_orientation_visitor() # the Python user has to del this
Pmp.set_polygon_orientation_reversed(v, dv)
Pmp.set_link_connected_polygons(v, g)

points, polygons = Pmp.orient_polygon_soup(points, polygons, {"visitor": v})

del v # don't forget to del :)

