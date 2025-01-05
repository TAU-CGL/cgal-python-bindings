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

# Reads a .xyz point set file in points[].
# Note: read_XYZ() requires an output iterator
# over points and as well as property maps to access each
# point position and normal.
points = []
success, points = CGALPY.read_points_with_normals(fname)
if not success:
    sys.stderr.write(f"Error: cannot read file {fname}\n")
    sys.exit(1)

# Saves point set.
if not CGALPY.write_points_with_normals("oni_copy.xyz", points, stream_precision=17):
    sys.exit(1)
