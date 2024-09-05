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

fname = sys.argv[i] if len(sys.argv)>i else CGALPY.data_file_path("points_3/sphere_1k.xyz")
i += 1

# Reads a point set file in points[].
sys.stderr.write("Open " + fname + " for reading...\n")

success, points = CGALPY.read_points_with_normals(fname)

if not success:
    sys.stderr.write("Error: cannot read file " + fname + "\n")
    sys.exit(1)

# Estimates normals direction.
# Note: pca_estimate_normals() requires a range of points
# as well as property maps to access each point's position and normal.
nb_neighbors = 18 # K-nearest neighbors = 3 rings

if len(sys.argv) > i and sys.argv[i] == "-r": # Use a fixed neighborhood radius
    # First compute a spacing using the K parameter
    spacing = CGALPY.compute_average_spacing_with_normals(points, nb_neighbors)
    # Then, estimate normals with a fixed radius
    points = CGALPY.pca_estimate_normals(points,
                                         0, # when using a neighborhood radius, K=0 means no limit on the number of neighbors returns
                                         neighbor_radius=2. * spacing) # use 2*spacing as neighborhood radius
else: # Use a fixed number of neighbors
    points = CGALPY.pca_estimate_normals(points, nb_neighbors)

# Orients normals.
# Note: mst_orient_normals() requires a range of points
# as well as property maps to access each point's position and normal.
points, unoriented_points_index = CGALPY.mst_orient_normals(points, nb_neighbors)
points = points[:unoriented_points_index]

