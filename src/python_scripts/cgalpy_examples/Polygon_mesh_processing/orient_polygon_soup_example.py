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

v = Pmp.Default_orientation_visitor()
# v.set_non_manifold_edge(lambda x: print(x)) #### Work in progress

points, polygons = Pmp.orient_polygon_soup(points, polygons, {"visitor": v})

