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
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

input_filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/blobby-shuffled.off")
i += 1
reference_filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/blobby.off")

try: points, polygons = Pol3.read_polygon_soup(input_filename)
except: raise ValueError("Error: can not read input file.")

if len(points) == 0 or len(polygons) == 0:
  print("Error: can not read input file.")

try: ref1 = Pol3.read_polygon_mesh(reference_filename)
except: raise ValueError("Invalid input.")

print(f"Is the soup a polygon mesh ? : {1 if Pmp.is_polygon_soup_a_polygon_mesh(polygons) else 0}")

points, polygons = Pmp.orient_triangle_soup_with_reference_triangle_mesh(ref1, points, polygons);

print(f"And now ? : {1 if Pmp.is_polygon_soup_a_polygon_mesh(polygons) else 0}")

polygons, points, duplicated = Pmp.duplicate_non_manifold_edges_in_polygon_soup(points, polygons);

print(f"And now ? : {1 if Pmp.is_polygon_soup_a_polygon_mesh(polygons) else 0}")

poly = Pmp.polygon_soup_to_polygon_mesh(points, polygons)[0]

fccmap = Pol3.get(Pol3.dynamic_property_face_size_t(), poly)

print(f"{Pmp.connected_components(poly, fccmap)} CCs before merge.")
Pmp.merge_reversible_connected_components(poly)
print(f"{Pmp.connected_components(poly, fccmap)} remaining CCs.")
