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

nb_neighbors = 24

# Estimate scale of the point set with average spacing.
average_spacing = Psp.compute_average_spacing(points, nb_neighbors)

# First option: use a distance threshold.
points, first_to_remove = Psp.remove_outliers(
  points, nb_neighbors,
  {"threshold_percent": 100.0,
   "threshold_distance": 2.0 * average_spacing})

print(f"{100.0 * first_to_remove / len(points):.2f}% of the points are kept " +
      f"when using a distance threshold of {2.0 * average_spacing:.6f}")

# Second option: use a removed percentage.
removed_percentage = 5.0
points, first_to_remove = Psp.remove_outliers(
  points, nb_neighbors,
  {"threshold_percent": removed_percentage,
   "threshold_distance": 0.0})

print(f"Keep {first_to_remove} point(s) after requesting {removed_percentage}% removal")
