import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

filename = 'meshes/mech-holes-shark.off' if len(sys.argv) < 2 else sys.argv[1]

mesh = Pol3.read_polygon_mesh(filename)

nb_holes = 0

for h in mesh.halfedges():
    if h.is_border():
        success, patch_facets, patch_vertices = Pmp.triangulate_refine_and_fair_hole(mesh, h, \
                                                                                    {'face_output_iterator': True, 'vertex_output_iterator': True})
        print(f" Number of facets in constructed patch: {len(patch_facets)}")
        print(f" Number of vertices in constructed patch: {len(patch_vertices)}")
        print(f" Fairing : {'succeeded' if success else 'failed'}")
        nb_holes += 1

print()
print(f"{nb_holes} holes have been filled")

Pol3.write_polygon_mesh('filled.off', mesh)

