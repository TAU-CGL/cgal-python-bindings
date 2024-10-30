import math
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
Ker = CGALPY.Ker

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/oni.pwn")
i += 1

point_set = CGALPY.Point_set_3()
if not CGALPY.read_point_set(fname, point_set, {"use_binary_mode": True}):
  print("Can't read input file")
  sys.exit(1)

if point_set.has_normal_map():
  for it in point_set:
    n = point_set.normal(it)
    n = -n / math.sqrt(n * n)
    point_set.set_normal(it, n)

if not CGALPY.write_point_set("normalized_normals.off", point_set, {"stream_precision": 17}):
  print("Can't write output file")
  sys.exit(1)

