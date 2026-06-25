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
  else CGALPY.data_file_path("points_3/before_upsample.xyz")
)
i += 1

output_filename = (
  sys.argv[i]
  if len(sys.argv) > i
  else "before_upsample_UPSAMPLED.xyz"
)

points = Psp.read_points_with_normals(input_filename)
if len(points) == 0:
  print("Error: cannot read file " + input_filename)
  sys.exit(1)

# Algorithm parameters.
sharpness_angle = 25.0
edge_sensitivity = 0.0
neighbor_radius = 0.25
number_of_output_points = len(points) * 4

points = Psp.edge_aware_upsample_point_set(
  points,
  {
    "sharpness_angle": sharpness_angle,
    "edge_sensitivity": edge_sensitivity,
    "neighbor_radius": neighbor_radius,
    "number_of_output_points": number_of_output_points,
  },
)

if not Psp.write_points_with_normals(output_filename, points):
  print("Error: cannot write file " + output_filename)
  sys.exit(1)

print(f"Upsampled to {len(points)} point(s) with normals")
print(f"Wrote {output_filename}")
