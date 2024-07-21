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


for i in range(2, len(sys.argv)):
    try:
        mesh = Sm.read_polygon_mesh(sys.argv[i])
    except:
        print(f"Invalid input: {sys.argv[i]}")
        continue
    if not Sm.is_triangle_mesh(mesh):
        print(sys.argv[i])

