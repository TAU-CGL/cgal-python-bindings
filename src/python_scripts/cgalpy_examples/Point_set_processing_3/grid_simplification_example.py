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

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/oni.pwn")
i += 1

# Reads a point set file in points.
success, points = CGALPY.read_points(fname)
if not success:
  print("Error: cannot read file " + fname)
  sys.exit(1)

# simplification by clustering using erase-remove idiom
cell_size = 0.03
min_points_per_cell = 3

points, first_to_remove = CGALPY.grid_simplify_point_set(points, cell_size, min_points_per_cell=min_points_per_cell)
points_removed = points[:first_to_remove]
