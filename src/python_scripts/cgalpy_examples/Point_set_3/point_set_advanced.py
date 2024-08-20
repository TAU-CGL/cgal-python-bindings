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
# CGALPY = importlib.import_module(lib)
import CGALPY.CGALPY as CGALPY

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/camel.off")

point_set = CGALPY.Point_set_3()
point_set.add_normal_map()

# Reading input in OFF format
if not CGALPY.read_point_set(filename, point_set):
  sys.stderr.write("Can't read input file\n")
  sys.exit(1)

