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

# Reads a polygon mesh file in points.
input_filename = CGALPY.data_file_path("meshes/fandisk_large.off")
success, points = CGALPY.read_points(input_filename)
if not success:
  print("Error: cannot read file " + input_filename)
  sys.exit(1)

# Estimates covariance matrices per points.
R = 0.2
r = 0.1

cov = CGALPY.compute_vcm(points, R, r)

# Find the points on the edges.
# Note that this step is not expensive and can be done several time to get better results
threshold = 0.16
output_filename = "points_on_edges.xyz"
# output = open(output_filename, "w")
with open(output_filename, "w") as output:
    i = 0
    for p in points:
      if CGALPY.vcm_is_on_feature_edge(cov[i], threshold):
        output.write(f"{p}\n")
      i += 1
