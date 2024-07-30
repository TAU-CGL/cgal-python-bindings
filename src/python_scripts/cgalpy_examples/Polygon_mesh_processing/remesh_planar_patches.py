import time
import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = 'meshes/cube_quad.off' if len(sys.argv) < 2 else sys.argv[1]

sm = Sm.read_polygon_mesh(filename)

Pmp.triangulate_faces(sm)
print(f"Input mesh has {len(list(sm.faces()))} faces")
assert len(list(sm.faces())) == 12

ecm = sm.add_property_map_edge_bool("ecm", False)[0]

Pmp.detect_sharp_edges(sm, 60, ecm)

Pmp.isotropic_remeshing(list(sm.faces()), 0.1, sm, {"edge_is_constrained_map": ecm})

Sm.write_polygon_mesh("cube_remeshed.off", sm, {"stream_precision": 17})
assert len(list(sm.faces())) > 100

out = Pmp.remesh_planar_patches(sm)

Sm.write_polygon_mesh("cube_decimated.off", out, {"stream_precision": 17})

print(f"Output mesh has {len(list(out.faces()))} faces")
assert len(list(out.faces())) == 12

