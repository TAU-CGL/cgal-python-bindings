import os
import sys
import importlib
if len(sys.argv) < 4:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[3]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename1 = 'meshes/blobby.off' if len(sys.argv) < 2 else sys.argv[1]
filename2 = 'meshes/eight.off' if len(sys.argv) < 3 else sys.argv[2]

try:
  mesh1 = Sm.read_polygon_mesh(filename1)
except:
  raise ValueError("Invalid input.")
try:
  mesh2 = Sm.read_polygon_mesh(filename2)
except:
  raise ValueError("Invalid input.")

try:
    out = Pmp.corefine_and_compute_union(mesh1, mesh2, {'visitor': Pmp.Corefine_visitor()})
except:
    raise ValueError("Cannot compute union!")

print("Union was successfully computed")
Sm.write_polygon_mesh("union.off", out);
