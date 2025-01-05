import numpy as np
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
Ker = CGALPY.Ker
Point_3 = Ker.Point_3

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/example.ply")
i += 1

point_set = CGALPY.Point_set_3()

if not CGALPY.read_point_set(fname, point_set, {"use_binary_mode": True}):
    sys.stderr.write("Can't read input file\n")
    sys.exit(1)

# Shows which properties are defined
properties = point_set.properties()
print("Properties:")
for i in range(len(properties)):
    print(f" * {properties[i]}")

# Recover "label" property of type int
label_prop = point_set.property_map_index("label")

if label_prop is not None:
    print("Point set has an integer \"label\" property with values:")
    for it in point_set:
        print(f" * {label_prop[it]}")

if sys.argv[-1] == "-b":
    CGALPY.write_point_set(fname, point_set, {"stream_precision": 17})
else:
    CGALPY.write_point_set(fname, point_set, {"stream_precision": 17, "use_binary_mode": False})
