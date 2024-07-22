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
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = "meshes/elephant.off" if len(sys.argv) < 3 else sys.argv[2]

mesh = Sm.read_polygon_mesh(filename)

Pmp.autorefine(mesh)

Sm.write_polygon_mesh("autorefined.off", mesh, {"stream_precision": 17})

