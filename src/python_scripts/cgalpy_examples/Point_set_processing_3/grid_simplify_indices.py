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

CGALPY = importlib.import_module(lib)

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/fin90_with_PCA_normals.xyz")
i += 1

# Reads a .xyz point set file in points.
success, points = CGALPY.read_points_with_normals(fname)
if not success:
  print("Error: cannot read file " + fname)
  sys.exit(1)

print(f"{len(points)} input points")
point_to_index = dict(zip(points, range(len(points))))

# simplification by clustering using erase-remove idiom
cell_size = 0.05
points, k = CGALPY.grid_simplify_point_set_with_normals(points, cell_size)

print(f"Keep {k} of {len(points)} points")

points, normals = zip(*points)

print(f"{k} points after simplification")
