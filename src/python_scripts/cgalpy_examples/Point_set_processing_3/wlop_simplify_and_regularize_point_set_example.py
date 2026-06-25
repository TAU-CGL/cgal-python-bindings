import time
import sys
import importlib

lib = 'CGALPY'
i = 1
if len(sys.argv) > 1 and sys.argv[1].startswith('CGALPY'):
  lib = sys.argv[1]
  i = 2

CGALPY = importlib.import_module(lib)
Psp = CGALPY.Psp

input_filename = (
  sys.argv[i]
  if len(sys.argv) > i
  else CGALPY.data_file_path("points_3/sphere_20k.xyz")
)
i += 1

output_filename = (
  sys.argv[i]
  if len(sys.argv) > i
  else "sphere_20k_WLOPED.xyz"
)

points = Psp.read_points(input_filename)
if len(points) == 0:
  print("Error: cannot read file " + input_filename)
  sys.exit(1)

select_percentage = 2.0
neighbor_radius = 0.5

timer = time.perf_counter()
output = Psp.wlop_simplify_and_regularize_point_set(
  points,
  {
    "select_percentage": select_percentage,
    "neighbor_radius": neighbor_radius,
  },
)
task_time = time.perf_counter() - timer

if not Psp.write_points(output_filename, output):
  print("Error: cannot write file " + output_filename)
  sys.exit(1)

print(f"WLOP simplification and regularization computed in {task_time:.2f} seconds")
print(f" * Input points: {len(points)}")
print(f" * Output points: {len(output)}")
print(f" * Select percentage: {select_percentage}")
print(f" * Neighbor radius: {neighbor_radius}")
print(f"Wrote {output_filename}")
