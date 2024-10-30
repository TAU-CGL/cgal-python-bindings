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
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

def is_small_hole(h, mesh, max_hole_diam, max_num_hole_edges):
  num_hole_edges = 0
  hole_bbox = Ker.Bbox_3()
  for hc in Sm.halfedges_around_face(h, mesh):
    p = mesh.point(Sm.target(hc, mesh))

    hole_bbox += p.bbox()
    num_hole_edges += 1

    if num_hole_edges > max_num_hole_edges:
      return False
    if hole_bbox.xmax() - hole_bbox.xmin() > max_hole_diam:
      return False
    if hole_bbox.ymax() - hole_bbox.ymin() > max_hole_diam:
      return False
    if hole_bbox.zmax() - hole_bbox.zmin() > max_hole_diam:
      return False

  return True

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/mech-holes-shark.off")
i += 1
max_hole_diam = float(sys.argv[i]) if len(sys.argv) > i else -1.0
i += 1
max_num_hole_edges = int(sys.argv[i]) if len(sys.argv) > i else -1

mesh = Sm.read_polygon_mesh(filename)

nb_holes = 0
border_cycles = Pmp.extract_boundary_cycles(mesh)

for h in border_cycles:
  if max_hole_diam > 0 and max_num_hole_edges > 0 and \
     not is_small_hole(h, mesh, max_hole_diam, max_num_hole_edges):
    continue

  # triangulate_refine_and_fair_hole returns a tuple of 3 elements:
  # - a boolean indicating if the fairing was successful
  # - if the "face_output_iterator" parameter is set, a list of the facets of the constructed patch
  # - if the "vertex_output_iterator" parameter is set, a list of the vertices of the constructed patch
  success, patch_facets, patch_vertices = Pmp.triangulate_refine_and_fair_hole(mesh, h,
                                                                                 {"face_output_iterator": True, "vertex_output_iterator": True})

  print("* Number of facets in constructed patch: ", len(patch_facets))
  print("  Number of vertices in constructed patch: ", len(patch_vertices))
  print("  Is fairing successful: ", success)
  nb_holes += 1

print()
print(f"{nb_holes} holes have been filled")
Sm.write_polygon_mesh("filled_SM.off", mesh, {"stream_precision": 17})
print("Mesh written to: filled_SM.off")
