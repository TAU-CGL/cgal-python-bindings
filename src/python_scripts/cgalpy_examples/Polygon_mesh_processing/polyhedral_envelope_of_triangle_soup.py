import os
import sys
import importlib

lib = 'CGALPY'
a = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    a = 2
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

points, polygons = Sm.read_polygon_soup("meshes/blobby.off")
eps = float(sys.argv[a]) if len(sys.argv) > a else 0.2
a += 1

envelope = Pmp.Polyhedral_envelope(points, polygons, eps)

i = int(sys.argv[a]) if len(sys.argv) > a else 0
j = int(sys.argv[a+1]) if len(sys.argv) > a+1 else 100
k = int(sys.argv[a+2]) if len(sys.argv) > a+2 else 200

if envelope.inside(points[i], points[j], points[k]):
  print("inside polyhedral envelope")

