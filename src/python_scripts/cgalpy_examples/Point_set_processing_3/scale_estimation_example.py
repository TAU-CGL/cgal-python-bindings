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

fname = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/sphere_20k.xyz")
i += 1

# read input
success, points = CGALPY.read_points(fname)


# estimate k scale
task_time = time.perf_counter()
k_scale = CGALPY.estimate_global_k_neighbor_scale(points)
task_time = time.perf_counter() - task_time
print("Global K scale:", k_scale)
print("Time:", task_time)

# Example: use estimated k as scale for jet smoothing
CGALPY.jet_smooth_point_set(points, k_scale)

# estimate range scale
timer = time.perf_counter()
range_scale = CGALPY.estimate_global_range_scale(points)
task_time += time.perf_counter() - timer

# Example: use estimated range for grid simplification
points, p = CGALPY.grid_simplify_point_set(points, range_scale)
points = points[:p]

memory = CGALPY.Memory_sizer().virtual_size()

print(f"Scales computed in {task_time} second(s) using " +
      f"{memory>>20} MiB of memory:")
print(f" * Global K scale: {k_scale}")
print(f" * Global range scale: {range_scale}")
