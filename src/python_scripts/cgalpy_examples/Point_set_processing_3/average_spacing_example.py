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

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/sphere_20k.xyz")
i += 1

# Reads a file in points.

status, points = CGALPY.read_points(fname)
if not status:
  sys.stderr.write("Error: cannot read file " + fname + "\n")
  sys.exit(1)

# Initialize index and RGB color fields in tuple.
# As the index and RGB color are respectively the first and third-fifth elements
# of the tuple we use a get function from the property map that accesses the 0
# and 2-4th elements of the tuple.

IndexedPointWithColorTupleList = [(i, point, 0, 0, 0) for i, point in enumerate(points)]

# Computes average spacing.
nb_neighbors = 6 # 1 ring
average_spacing = CGALPY.compute_average_spacing(tuple(map(lambda x: x[1], IndexedPointWithColorTupleList)), nb_neighbors)

print(f"Average spacing: {average_spacing}")
