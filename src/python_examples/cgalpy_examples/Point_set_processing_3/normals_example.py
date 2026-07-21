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

args = sys.argv[i:]
use_radius = False
fname = CGALPY.data_file_path("points_3/fin90_with_PCA_normals.xyz")

if len(args) > 0 and args[0] == "-r":
  use_radius = True
  args = args[1:]

if len(args) > 0:
  fname = args[0]
  args = args[1:]

if len(args) > 0 and args[0] == "-r":
  use_radius = True

# Reads a point set file with normals.
sys.stderr.write("Open " + fname + " for reading...\n")

points = Psp.read_points_with_normals(fname)

if len(points) == 0:
  sys.stderr.write("Error: cannot read file " + fname + "\n")
  sys.exit(1)

# Estimates normals direction.
nb_neighbors = 18

if use_radius:
  # First compute a spacing using the K parameter.
  spacing = Psp.compute_average_spacing_with_normals(points, nb_neighbors)

  # Then estimate normals with a fixed radius.
  points = Psp.pca_estimate_normals(
    points, 0, {"neighbor_radius": 2.0 * spacing})
else:
  # Use a fixed number of neighbors.
  points = Psp.pca_estimate_normals(points, nb_neighbors)

# Orients normals.
points, unoriented_points_index = Psp.mst_orient_normals(points, nb_neighbors)
points = points[:unoriented_points_index]

print(f"Read {len(points)} oriented point(s)")
print(f"First unoriented point index: {unoriented_points_index}")
