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

fname = sys.argv[i] if len(sys.argv)>i else CGALPY.data_file_path("points_3/oni.pwn")
i += 1

# Reads a point set file in points[].
success, points = CGALPY.read_points(fname)
if not success:
    sys.stderr.write(f"Error: cannot read file {fname}\n")
    sys.exit(1)

# Randomly simplifies using erase-remove idiom
removed_percentage = 97.0 # percentage of points to remove
points, removed_points_index = CGALPY.random_simplify_point_set(points, removed_percentage)
points = points[:removed_points_index]

# Saves point set.
output_filename = sys.argv[i] if len(sys.argv)>i else "Three_lady_copy.xyz"
i += 1
if not CGALPY.write_points(output_filename, points, stream_precision=17):
    sys.stderr.write("Error: cannot write file " + output_filename + "\n")
    sys.exit(1)
