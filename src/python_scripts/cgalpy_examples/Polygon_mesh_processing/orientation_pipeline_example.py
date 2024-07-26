#!/usr/bin/python
import os
import sys
import importlib
if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

input_filename = "meshes/blobby-shuffled.off" if len(sys.argv) < 3 else sys.argv[2]
reference_filename = "meshes/blobby.off" if len(sys.argv) < 4 else sys.argv[3]

try:
  points, polygons = Pol3.read_polygon_soup(input_filename)
except:
  print("Error: can not read input file.")
  exit(1)

if len(points) == 0 or len(polygons) == 0:
  print("Error: can not read input file.")

try:
  ref1 = Pol3.read_polygon_mesh(reference_filename)
except:
  print("Invalid input.")
  exit(1)

print(f"Is the soup a polygon mesh ? : {1 if Pmp.is_polygon_soup_a_polygon_mesh(polygons) else 0}")

polygons = Pmp.orient_triangle_soup_with_reference_triangle_mesh(ref1, points, polygons);

print(f"And now ? : {1 if Pmp.is_polygon_soup_a_polygon_mesh(polygons) else 0}")

polygons, points, duplicated = Pmp.duplicate_non_manifold_edges_in_polygon_soup(points, polygons);

print(f"And now ? : {1 if Pmp.is_polygon_soup_a_polygon_mesh(polygons) else 0}")

poly = Pmp.polygon_soup_to_polygon_mesh(points, polygons)[0]

fccmap = Pol3.get(Pol3.dynamic_property_face_size_t(), poly)

print(f"{Pmp.connected_components(poly, fccmap)} CCs before merge.")
Pmp.merge_reversible_connected_components(poly)
print(f"{Pmp.connected_components(poly, fccmap)} remaining CCs.")

