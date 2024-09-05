import time
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

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/oni.pwn")
i += 1

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/oni.pwn")
i += 1

# Reads a point set file in points[].
success, points = CGALPY.read_points(fname)
if not success:
  print("Error: cannot read file " + fname)
  sys.exit(1)

print(f"Read {len(points)} point(s)")

timer = time.perf_counter()

# simplification by clustering using erase-remove idiom
points, p = CGALPY.hierarchy_simplify_point_set(points)
points = points[:p]

memory = CGALPY.Memory_sizer().virtual_size()

print(f"{len(points)} point(s) kept, computed in " +
      f"{time.perf_counter() - timer:.2f} seconds, " +
      f"{memory>>20} Mib allocated.")

CGALPY.write_points("out.xyz", points, stream_precision=17)
