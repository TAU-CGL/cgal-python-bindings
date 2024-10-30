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

tmesh = Sm.read_polygon_mesh("meshes/blobby.off")

query = tmesh
longest_edge = max(query.edges(), key=lambda e: Pmp.edge_length(query.halfedge(e), query))

target_edge_length = Pmp.edge_length(query.halfedge(longest_edge), query)
Pmp.isotropic_remeshing(list(query.faces()), Pmp.Uniform_sizing_field(target_edge_length, query), query)

# construct the polyhedral envelope
eps = float(sys.argv[a]) if len(sys.argv) > a else 0.01
a += 1
envelope = Pmp.Polyhedral_envelope(tmesh, eps)

if envelope.inside(query):
  print("Remeshing is inside the polyhedral envelope")
else:
  print("Remeshing is not inside the polyhedral envelope")

Sm.write_polygon_mesh("remeshed.off", query)

