#!/usr/bin/python

import os
import sys
import importlib
lib = 'CGALPY'
i = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    i = 2
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))

CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/mech-holes-shark.off")

mesh = Pol3.read_polygon_mesh(filename)

nb_holes = 0

for h in Pol3.halfedges(mesh):
  if h.is_border():

    # triangulate_refine_and_fair_hole returns a tuple of 3 elements:
    # - a boolean indicating if the fairing was successful
    # - if the "face_output_iterator" parameter is set, a list of the facets of the constructed patch
    # - if the "vertex_output_iterator" parameter is set, a list of the vertices of the constructed patch
    success, patch_facets, patch_vertices = Pmp.triangulate_refine_and_fair_hole(mesh, h, \
      {'face_output_iterator': True, 'vertex_output_iterator': True})
    print(f" Number of facets in constructed patch: {len(patch_facets)}")
    print(f" Number of vertices in constructed patch: {len(patch_vertices)}")
    print(f" Fairing : {'succeeded' if success else 'failed'}")
    nb_holes += 1

print()
print(f"{nb_holes} holes have been filled")

Pol3.write_polygon_mesh('filled.off', mesh)
