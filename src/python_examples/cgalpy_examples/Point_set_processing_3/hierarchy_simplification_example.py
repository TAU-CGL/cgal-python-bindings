import time
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

print(f"Read {len(points)} point(s)")

timer = time.perf_counter()

# Simplification by clustering using erase-remove idiom.
points, first_to_remove = Psp.hierarchy_simplify_point_set(
  points, {"size": 10, "maximum_variation": 1.0 / 3.0})

print(f"Keep {first_to_remove} point(s), computed in " +
      f"{time.perf_counter() - timer:.2f} second(s)")
print(f"Remove {len(points) - first_to_remove} point(s)")
