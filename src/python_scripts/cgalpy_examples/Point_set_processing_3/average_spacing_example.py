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

# Reads a file in points.
points = Psp.read_points(fname)
if len(points) == 0:
  sys.stderr.write("Error: cannot read file " + fname + "\n")
  sys.exit(1)

# Computes average spacing.
nb_neighbors = 6 # 1 ring
average_spacing = Psp.compute_average_spacing(points, nb_neighbors)

print(f"Average spacing: {average_spacing}")
