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

fname = sys.argv[i] if len(sys.argv)>i else CGALPY.data_file_path("points_3/oni.pwn")
i += 1

# Reads a .xyz point set file in points[].
# Note: read_XYZ() requires an output iterator
# over points and as well as property maps to access each
# point position and normal.
points = []
success, points = CGALPY.read_points_with_normals(fname)
if not success:
    sys.stderr.write(f"Error: cannot read file {fname}\n")
    sys.exit(1)

# Removes outliers using erase-remove idiom.
# The Identity_property_map property map can be omitted here as it is the default value.
nb_neighbors = 24 # considers 24 nearest neighbor points

# Estimate scale of the point set with average spacing
average_spacing = CGALPY.compute_average_spacing_with_normals(points, nb_neighbors)

##################
# FIRST OPTION #
# I don't know the ratio of outliers present in the point set
points, first_to_remove = CGALPY.remove_outliers(points,
 nb_neighbors,
 threshold_percent=100., # No limit on the number of outliers to remove
 threshold_distance=2. * average_spacing) # Point with distance above 2*average_spacing are considered remove_outliers

sys.stderr.write(f"{100. * first_to_remove / len(points):.2f}" +
                 f"% of the points are considered outliers when using a distance threshold of " +
                 f"{2. * average_spacing:.2f}\n")

##################
# SECOND OPTION #
# I know the ratio of outliers present in the point set
removed_percentage = 5.0
points, first_to_remove = CGALPY.remove_outliers(points,
 nb_neighbors,
 threshold_percent=removed_percentage, # Minimum percentage to remove
 threshold_distance=0.) # No distance threshold (can be omitted)
points = points[:first_to_remove]

