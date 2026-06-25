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
  else CGALPY.data_file_path("points_3/fin90_with_PCA_normals.xyz")
)
i += 1

output_filename = (
  sys.argv[i]
  if len(sys.argv) > i
  else "fin90_with_PCA_normals_bilateral_smoothed.xyz"
)

points = Psp.read_points_with_normals(input_filename)
if len(points) == 0:
  sys.stderr.write("Error: cannot read file " + input_filename + "\n")
  sys.exit(1)

# Algorithm parameters.
k = 120
sharpness_angle = 1.0
iter_number = 3

last_error = 0.0
for _ in range(iter_number):
  last_error, points = Psp.bilateral_smooth_point_set(
    points, k, {"sharpness_angle": sharpness_angle})

if not Psp.write_points_with_normals(output_filename, points):
  sys.stderr.write("Error: cannot write file " + output_filename + "\n")
  sys.exit(1)

print(f"Read {len(points)} point(s) with normals")
print(f"Bilateral smoothing iterations: {iter_number}")
print(f"Last average movement error: {last_error}")
print(f"Wrote {output_filename}")
