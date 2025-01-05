import time
import random
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

input_filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/hippo1.ply")
i += 1

points = CGALPY.Point_set_3_3()
# Read input file
CGALPY.read_point_set(input_filename, points)

# Add a cluster map
cluster_map = points.add_property_map_int("cluster", -1)[0]

# Compute average spacing
spacing = CGALPY.compute_average_spacing(points, 12)
print(f"Spacing = {spacing}")

# Adjacencies stored in vector
# Compute clusters
t = time.perf_counter_ns()
nb_clusters, adjacencies = CGALPY.cluster_point_set(points, cluster_map, neighbor_radius=spacing)
t = time.perf_counter_ns() - t
print(f"Found {nb_clusters} clusters with {len(adjacencies)} adjacencies in {t/1e9} seconds")

# Output a colored PLY file
red = points.add_property_map_unsigned_char("red", 0)[0]
green = points.add_property_map_unsigned_char("green", 0)[0]
blue = points.add_property_map_unsigned_char("blue", 0)[0]
for idx in points:
  # One color per cluster
  red[idx] = random.randint(64, 192)
  green[idx] = random.randint(64, 192)
  blue[idx] = random.randint(64, 192)

CGALPY.write_point_set("out.ply", points, use_binary_mode = True)
