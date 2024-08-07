#!/usr/bin/python

import time
import os
import sys
import importlib
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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

MAX_UINT_64 = 2**64 - 1

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/fandisk.off')

sm = Sm.read_polygon_mesh(filename)

# apply a perturbation to input vertices so that points are no longer coplanar
Pmp.random_perturbation(sm, 0.001)

region_ids = sm.add_property_map_face_size_t("region_ids")[0]
corner_id_map = sm.add_property_map_vertex_size_t("corner_id_map", MAX_UINT_64)[0]
ecm = sm.add_property_map_edge_bool("ecm", False)[0]
normal_map = Sm.Vector_vector_3_map()

# detect planar regions in the mesh
nb_regions = Pmp.region_growing_of_planes_on_faces(sm,
                                                   region_ids,
                                                   {"cosine_of_maximum_angle": 0.98,
                                                    "region_primitive_map": normal_map,
                                                    "maximum_distance": 0.011})

# detect corner vertices on the boundary of planar regions
nb_corners = Pmp.detect_corners_of_regions(sm,
                                           region_ids,
                                           nb_regions,
                                           corner_id_map,
                                           {"cosine_of_maximum_angle": 0.98,
                                            "maximum_distance": 0.011,
                                            "edge_is_constrained_map": ecm})

# run the remeshing algorithm using filled properties
out = Pmp.remesh_almost_planar_patches(sm,
                                 nb_regions, nb_corners,
                                 region_ids,
                                 corner_id_map,
                                 ecm,
                                 {"patch_normal_map": normal_map})

Sm.write_polygon_mesh("fandisk_remeshed.off", out)
