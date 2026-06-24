import time
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

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/sphere_20k.xyz")

# Read input points.
points = Psp.read_points(fname)
if len(points) == 0:
  sys.stderr.write("Error: cannot read file " + fname + "\n")
  sys.exit(1)

# Estimate global scales.
task_time = time.perf_counter()
k_scale = Psp.estimate_global_k_neighbor_scale(points)
range_scale = Psp.estimate_global_range_scale(points)
task_time = time.perf_counter() - task_time

# Estimate local scales for a small query subset.
queries = points[:3]
local_k_scales = Psp.estimate_local_k_neighbor_scales(points, queries)
local_range_scales = Psp.estimate_local_range_scales(points, queries)

print(f"Scales computed in {task_time} second(s):")
print(f" * Global K scale: {k_scale}")
print(f" * Global range scale: {range_scale}")
print(f" * Local K scales for first {len(queries)} points: {local_k_scales}")
print(f" * Local range scales for first {len(queries)} points: {local_range_scales}")
