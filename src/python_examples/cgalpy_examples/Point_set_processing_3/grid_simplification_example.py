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
Psp = CGALPY.Psp

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/sphere_20k.xyz")

# Reads a point set file in points.
points = Psp.read_points(fname)
if len(points) == 0:
  print("Error: cannot read file " + fname)
  sys.exit(1)

# Simplification by clustering using erase-remove idiom.
cell_size = 0.03
min_points_per_cell = 3

points, first_to_remove = Psp.grid_simplify_point_set(
  points, cell_size, {"min_points_per_cell": min_points_per_cell})

print(f"Read {len(points)} point(s)")
print(f"Keep {first_to_remove} point(s)")
print(f"Remove {len(points) - first_to_remove} point(s)")
