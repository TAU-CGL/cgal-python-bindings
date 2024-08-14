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
p: Ker.Point_3
p = Ker.Kernel.Point_3(CGALPY.Ker.FT(1), CGALPY.Ker.FT(2), CGALPY.Ker.FT(3))
print(p)

