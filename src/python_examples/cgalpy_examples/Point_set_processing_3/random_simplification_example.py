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
  sys.stderr.write(f"Error: cannot read file {fname}\n")
  sys.exit(1)

# Randomly simplifies using erase-remove idiom.
removed_percentage = 97.0
points, first_to_remove = Psp.random_simplify_point_set(points, removed_percentage)

print(f"Read {len(points)} point(s)")
print(f"Keep {first_to_remove} point(s)")
print(f"Remove {len(points) - first_to_remove} point(s)")
