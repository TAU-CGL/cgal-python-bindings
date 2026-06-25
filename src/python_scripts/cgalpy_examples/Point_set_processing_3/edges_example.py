import sys
import importlib

lib = 'CGALPY'
if len(sys.argv) > 1 and sys.argv[1].startswith('CGALPY'):
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Psp = CGALPY.Psp

# Reads a point set file.
input_filename = CGALPY.data_file_path("meshes/fandisk_large.off")
points = Psp.read_points(input_filename)

if len(points) == 0:
  print("Error: cannot read file " + input_filename)
  sys.exit(1)

# Estimates covariance matrices per point.
R = 0.2
r = 0.1
cov = Psp.compute_vcm(points, R, r)

# Finds points on edges.
# This step is not expensive and can be done several times to tune the threshold.
threshold = 0.16
edge_count = 0

for i, p in enumerate(points):
  if Psp.vcm_is_on_feature_edge(cov[i], threshold):
    edge_count += 1

print(f"Read {len(points)} point(s)")
print(f"Detected {edge_count} edge point(s)")
