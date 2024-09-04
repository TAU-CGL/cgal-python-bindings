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

input_filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/fin90_with_PCA_normals.xyz")
i += 1
output_filename = sys.argv[i] if len(sys.argv) > i else "data/fin90_with_PCA_normals_bilateral_smoothed.xyz"
i += 1

# Reads a point set file in points[] * with normals *.
result, points = CGALPY.read_points_with_normals(input_filename)
if not result:
  sys.stderr.write("Error: cannot read file " + input_filename + "\n")
  sys.exit(1)

# Algorithm parameters
k = 120 # size of neighborhood. The bigger the smoother the result will be.
# This value should bigger than 1.
sharpness_angle = 1 # control sharpness of the result.
# The bigger the smoother the result will be
iter_number = 3 # number of times the projection is applied

for i in range(iter_number):
  # error =
  CGALPY.bilateral_smooth_point_set(points, k, sharpness_angle=sharpness_angle)

if not CGALPY.write_points(output_filename, points):
  sys.stderr.write("Error: cannot write file " + output_filename + "\n")
  sys.exit(1)

