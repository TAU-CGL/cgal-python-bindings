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

CGALPY = importlib.import_module(lib)

input_filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/sphere_20k.xyz")
i += 1
output_filename = sys.argv[i] if len(sys.argv) > i else "data/sphere_20k_WLOPED.xyz"

# Reads a .xyz point set file in points[]
success, points = CGALPY.read_points(input_filename)


# parameters
retain_percentage = 2   # percentage of points to retain.
neighbor_radius = 0.5   # neighbors size.

timer = time.perf_counter()
points, output = CGALPY.wlop_simplify_and_regularize_point_set(points, retain_percentage=retain_percentage, neighbor_radius=neighbor_radius)
task_time = time.perf_counter() - timer

CGALPY.write_points(output_filename, output)

memory = CGALPY.Memory_sizer().virtual_size()

print(f"WLOP Simplification and Regularization computed in {task_time:.2f} seconds"
      f"{memory>>20} MiB of memory:")
print(f" * Retain percentage: {retain_percentage}")
print(f" * Neighbor radius: {neighbor_radius}")
