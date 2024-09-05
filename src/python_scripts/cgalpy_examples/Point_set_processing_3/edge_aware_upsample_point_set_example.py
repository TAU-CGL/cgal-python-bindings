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

input_filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/before_upsample.xyz")
i += 1
output_filename = sys.argv[i] if len(sys.argv) > i else "data/before_upsample_UPSAMPLED.xyz"
i += 1

# Reads a .xyz point set file in points[], *with normals*.
success, points = CGALPY.read_points_with_normals(input_filename)
if not success:
  print("Error: cannot read file " + input_filename)
  sys.exit(1)

#Algorithm parameters
sharpness_angle = 25   # control sharpness of the result.
edge_sensitivity = 0    # higher values will sample more points near the edges
neighbor_radius = 0.25  # initial size of neighborhood.
number_of_output_points = len(points) * 4

   #Run algorithm
CGALPY.edge_aware_upsample_point_set(points, sharpness_angle=sharpness_angle, edge_sensitivity=edge_sensitivity, neighbor_radius=neighbor_radius, number_of_output_points=number_of_output_points)

# Saves point set.
success = CGALPY.write_points(output_filename, points)
if not success:
  print("Error: cannot write file " + output_filename)
  sys.exit(1)
