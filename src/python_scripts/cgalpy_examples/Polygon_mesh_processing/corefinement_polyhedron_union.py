#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
from typing import Any

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Pmp = CGALPY.Pmp

try: filename1 = argv[1]
except: filename1 = 'meshes/blobby.off'
try: filename2 = argv[2]
except: filename2 = 'meshes/eight.off'
# print(filename1, filename2)

Pm: Any = None
def p3():
  global Pm
  try: Pm = CGALPY.Pol3
  except: return False
  else: return True

def sm3():
  global Pm
  try: Pm = CGALPY.Sm
  except: return False
  else: return True

if not (p3() or sm3()): raise ValueError("Cannot find a polygonal mesh.")

# typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Mesh;

try:
  mesh1 = Pm.read_polygon_mesh(filename1)
  mesh2 = Pm.read_polygon_mesh(filename2)
except:
  raise ValueError("Invalid input.")

try:
  out = Pmp.corefine_and_compute_union(mesh1, mesh2)
except:
  raise ValueError("Union could not be computed.")

print("Union was successfully computed")
out_file = open("union.off", 'w')
out_file.write(str(out))
out_file.close()
