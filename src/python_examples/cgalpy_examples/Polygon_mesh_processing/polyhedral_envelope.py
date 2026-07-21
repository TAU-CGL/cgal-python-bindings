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

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

tmesh = Sm.read_polygon_mesh(CGALPY.data_file_path("meshes/blobby.off"))

eps = sys.argv[a] if len(sys.argv) > a else 0.2
a += 1

envelope = Pmp.Polyhedral_envelope(tmesh, eps)

i = sys.argv[a] if len(sys.argv) > a else 0
a += 1
j = sys.argv[a] if len(sys.argv) > a else 100
a += 1
k = sys.argv[a] if len(sys.argv) > a else 200

if envelope.inside(tmesh.point(Sm.Vertex_descriptor(i)),
            tmesh.point(Sm.Vertex_descriptor(j)),
            tmesh.point(Sm.Vertex_descriptor(k))):
    print("inside polyhedral envelope")
